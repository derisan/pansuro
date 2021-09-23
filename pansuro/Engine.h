#pragma once

#include "Core.h"

#include "PipelineState.h"

class Engine
{
public:
	Engine(UINT width, UINT height, std::wstring title);
	~Engine();

	void OnInit();
	void OnDestroy();
	void OnUpdate();
	void OnRender();

	void OnKeyDown(UINT8 keycode);
	void OnKeyUp(UINT8 keycode);

	UINT GetWidth() const { return m_Width; }
	UINT GetHeight() const { return m_Height; }
	float GetAspectRatio() const { return m_AspectRatio; }
	const std::wstring& GetTitle() const { return m_Title; }

	ComPtr<ID3D12Device> GetDevice() { return m_Device; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return m_CmdList; }

	static Engine* Instance() { return s_Instance; }

private:
	void LoadPipeline();
	void LoadAssets();
	void CreateRootSignature();

	void MoveToNextFrame();
	void WaitForGpu();

	void BeginRender();
	void EndRender();

	void CreateTestTriangle();

private:
	static Engine* s_Instance;

	static const UINT kFrameCount = 2;

	// Window properties.
	UINT m_Width;
	UINT m_Height;
	float m_AspectRatio;
	std::wstring m_Title;

	// DirectX12 
	CD3DX12_VIEWPORT m_Viewport;
	CD3DX12_RECT m_ScissorRect;
	ComPtr<IDXGISwapChain3> m_SwapChain;
	ComPtr<ID3D12Device> m_Device;
	ComPtr<ID3D12Resource> m_RenderTargets[kFrameCount];
	ComPtr<ID3D12CommandAllocator> m_CmdAllocators[kFrameCount];
	ComPtr<ID3D12CommandQueue> m_CmdQueue;
	ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	ComPtr<ID3D12GraphicsCommandList> m_CmdList;
	ComPtr<ID3D12Fence> m_Fence;
	ComPtr<ID3D12RootSignature> m_RootSignature;

	std::unique_ptr<PipelineState> m_PSO;

	UINT m_RtvDescriptorSize;
	UINT m_FrameIndex;
	HANDLE m_FenceEvent;
	UINT64 m_FenceValues[kFrameCount];

	// For test
	ComPtr<ID3D12Resource> m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
};

#define DEVICE Engine::Instance()->GetDevice()
#define CMD_LIST Engine::Instance()->GetCmdList()