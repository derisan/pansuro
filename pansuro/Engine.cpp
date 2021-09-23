#include "pch.h"
#include "Engine.h"

#include "Application.h"
#include "Mesh.h"
#include "Scene.h"
#include "TextureDescriptorHeap.h"


Engine* Engine::s_Instance = nullptr;
TextureDescriptorHeap* Engine::s_TextureDescriptorHeap = nullptr;

Engine::Engine(UINT width, UINT height, std::wstring title)
	: m_Width(width), m_Height(height), m_Title(title)
	, m_RtvDescriptorSize(0), m_FrameIndex(0), m_FenceEvent(nullptr)
	, m_FenceValues{}
	, m_ActiveScene(nullptr)
{
	MK_ASSERT(!s_Instance, "Engine's initialized more than once.");
	s_Instance = this;

	m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

	m_Viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
	m_ScissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));

	// Create Scene
	m_ActiveScene = new Scene();
}

Engine::~Engine()
{

}

Engine* Engine::CreateEngine(UINT width, UINT height, std::wstring title)
{
	return new Engine(width, height, title);
}

void Engine::OnInit()
{
	Log::Init();

	MK_INFO("Engine Initializing...");

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	LoadPipeline();
	LoadAssets();
}

void Engine::OnDestroy()
{
	WaitForGpu();
	CloseHandle(m_FenceEvent);

	if (m_ActiveScene)
	{
		m_ActiveScene->OnDestroy();
		delete m_ActiveScene;
	}
}

void Engine::OnUpdate()
{
	m_ActiveScene->OnUpdate();
}

void Engine::OnRender()
{
	BeginRender();

	m_ActiveScene->OnRender();

	EndRender();
}

void Engine::OnKeyDown(UINT8 keycode)
{
	if (keycode == VK_ESCAPE)
	{
		::PostQuitMessage(0);
	}

	m_ActiveScene->OnKeyDown(keycode);
}

void Engine::OnKeyUp(UINT8 keycode)
{
	m_ActiveScene->OnKeyUp(keycode);
}

void Engine::Submit(Mesh* mesh)
{
	m_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CmdList->IASetVertexBuffers(0, 1, mesh->GetVertexBufferView());
	m_CmdList->IASetIndexBuffer(mesh->GetIndexBufferView());
	m_CmdList->DrawIndexedInstanced(mesh->GetIndexCount(), 1, 0, 0, 0);
}

void Engine::LoadPipeline()
{
	UINT dxgiFactoryFlags = 0;

#ifdef MK_DEBUG
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
	swapChainDesc.Width = m_Width;
	swapChainDesc.Height = m_Height;
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

	{
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_Width, m_Height);
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		const auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		const auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
		m_Device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&m_DsvBuffer));

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		m_Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_DsvHeap));
		m_Device->CreateDepthStencilView(m_DsvBuffer.Get(), nullptr, m_DsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	s_TextureDescriptorHeap = new TextureDescriptorHeap();
}

void Engine::LoadAssets()
{
	CreateRootSignature();

	ThrowIfFailed(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_CmdAllocators[m_FrameIndex].Get(), m_PSO->GetPSO().Get(), IID_PPV_ARGS(&m_CmdList)));

	// Load things here.
	m_ActiveScene->OnInit();

	// Flush command queue for resource copying.
	ThrowIfFailed(m_CmdList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_CmdList.Get() };
	m_CmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	ThrowIfFailed(m_Device->CreateFence(m_FenceValues[m_FrameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
	m_FenceValues[m_FrameIndex]++;

	m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_FenceEvent == nullptr)
	{
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}

	WaitForGpu();

	for (auto buffer : m_UsedUploadBuffers)
	{
		//SAFE_RELEASE(buffer);
		if (buffer) 
			buffer->Release();
	}
}

void Engine::CreateRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE descRange[1];
	descRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER params[3];
	params[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	params[1].InitAsDescriptorTable(_countof(descRange), descRange, D3D12_SHADER_VISIBILITY_PIXEL);
	params[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	const auto samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(params),
		params, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
	ThrowIfFailed(m_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputDesc;
	inputDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	m_PSO = std::make_unique<PipelineState>(L"Assets/Shaders/shader.hlsli", inputDesc, m_RootSignature);
}

void Engine::MoveToNextFrame()
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

void Engine::WaitForGpu()
{
	ThrowIfFailed(m_CmdQueue->Signal(m_Fence.Get(), m_FenceValues[m_FrameIndex]));

	ThrowIfFailed(m_Fence->SetEventOnCompletion(m_FenceValues[m_FrameIndex], m_FenceEvent));
	WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);

	m_FenceValues[m_FrameIndex]++;
}

void Engine::BeginRender()
{
	ThrowIfFailed(m_CmdAllocators[m_FrameIndex]->Reset());
	ThrowIfFailed(m_CmdList->Reset(m_CmdAllocators[m_FrameIndex].Get(), m_PSO->GetPSO().Get()));

	m_CmdList->SetGraphicsRootSignature(m_RootSignature.Get());
	m_CmdList->RSSetViewports(1, &m_Viewport);
	m_CmdList->RSSetScissorRects(1, &m_ScissorRect);
	
	const auto toRenderTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CmdList->ResourceBarrier(1, &toRenderTargetBarrier);

	CD3DX12_CPU_DESCRIPTOR_HANDLE renderTagetView(m_RtvHeap->GetCPUDescriptorHandleForHeapStart(), m_FrameIndex, m_RtvDescriptorSize);
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_CmdList->ClearRenderTargetView(renderTagetView, Colors::CornflowerBlue, 0, nullptr);
	m_CmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	m_CmdList->OMSetRenderTargets(1, &renderTagetView, FALSE, &depthStencilView);
}

void Engine::EndRender()
{
	const auto toRenderTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CmdList->ResourceBarrier(1, &toRenderTargetBarrier);

	ThrowIfFailed(m_CmdList->Close());

	ID3D12CommandList* ppCommandLists[] = { m_CmdList.Get() };
	m_CmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	ThrowIfFailed(m_SwapChain->Present(1, 0));

	MoveToNextFrame();
}
