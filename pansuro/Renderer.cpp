#include "pch.h"
#include "Renderer.h"

#include "Engine.h"
#include "Application.h"

Renderer* Renderer::s_Instance = nullptr;

Renderer* Renderer::CreateRenderer(Engine* engine)
{
	return new Renderer(engine);
}

Renderer::Renderer(Engine* engine)
	: m_Engine(engine)
	, m_RtvDescriptorSize(0)
	, m_FrameIndex(0)
	, m_FenceEvent(nullptr)
	, m_FenceValues{}
{
	MK_ASSERT(!s_Instance, "Renderer already exists.");
	s_Instance = this;
}

Renderer::~Renderer()
{

}

void Renderer::LoadPipeline()
{
	UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif 

	ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));
	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CmdQueue)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = kFrameCount;
	swapChainDesc.Width = m_Engine->GetWidth();
	swapChainDesc.Height = m_Engine->GetHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		m_CmdQueue.Get(),
		Application::GetHwnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));

	ThrowIfFailed(factory->MakeWindowAssociation(Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));
	ThrowIfFailed(swapChain.As(&m_SwapChain));
	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = kFrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap)));

		m_RtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());

		for (UINT n = 0; n < kFrameCount; n++)
		{
			ThrowIfFailed(m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])));
			m_Device->CreateRenderTargetView(m_RenderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_RtvDescriptorSize);

			ThrowIfFailed(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CmdAllocators[n])));
		}
	}
}

void Renderer::LoadAssets()
{
	ThrowIfFailed(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_CmdAllocators[m_FrameIndex].Get(), /*m_PipelineState.Get()*/nullptr, IID_PPV_ARGS(&m_CmdList)));

	ThrowIfFailed(m_CmdList->Close());

	ThrowIfFailed(m_Device->CreateFence(m_FenceValues[m_FrameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
	m_FenceValues[m_FrameIndex]++;

	m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_FenceEvent == nullptr)
	{
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}

	WaitForGpu();
}

void Renderer::MoveToNextFrame()
{
	const UINT64 currentFenceValue = m_FenceValues[m_FrameIndex];
	ThrowIfFailed(m_CmdQueue->Signal(m_Fence.Get(), currentFenceValue));

	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	if (m_Fence->GetCompletedValue() < m_FenceValues[m_FrameIndex])
	{
		ThrowIfFailed(m_Fence->SetEventOnCompletion(m_FenceValues[m_FrameIndex], m_FenceEvent));
		WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
	}

	m_FenceValues[m_FrameIndex] = currentFenceValue + 1;
}

void Renderer::WaitForGpu()
{
	ThrowIfFailed(m_CmdQueue->Signal(m_Fence.Get(), m_FenceValues[m_FrameIndex]));

	ThrowIfFailed(m_Fence->SetEventOnCompletion(m_FenceValues[m_FrameIndex], m_FenceEvent));
	WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);

	m_FenceValues[m_FrameIndex]++;
}

void Renderer::BeginRender()
{
	ThrowIfFailed(m_CmdAllocators[m_FrameIndex]->Reset());
	ThrowIfFailed(m_CmdList->Reset(m_CmdAllocators[m_FrameIndex].Get(), /*m_PipelineState.Get()*/nullptr));

	m_CmdList->RSSetViewports(1, &m_Viewport);
	m_CmdList->RSSetScissorRects(1, &m_ScissorRect);

	const auto toRenderTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CmdList->ResourceBarrier(1, &toRenderTargetBarrier);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart(), m_FrameIndex, m_RtvDescriptorSize);
	m_CmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	m_CmdList->ClearRenderTargetView(rtvHandle, Colors::CornflowerBlue, 0, nullptr);
}

void Renderer::EndRender()
{
	const auto toPresentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_CmdList->ResourceBarrier(1, &toPresentBarrier);

	ThrowIfFailed(m_CmdList->Close());

	ID3D12CommandList* ppCommandLists[] = { m_CmdList.Get() };
	m_CmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	ThrowIfFailed(m_SwapChain->Present(1, 0));

	MoveToNextFrame();
}

void Renderer::OnInit()
{
	ThrowIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));
	LoadPipeline();
	LoadAssets();
}

void Renderer::OnDestroy()
{
	WaitForGpu();
	CloseHandle(m_FenceEvent);
}

void Renderer::OnRender()
{
	BeginRender();

	// m_Engine->GetActiveScene()->OnRender()

	EndRender();
}


