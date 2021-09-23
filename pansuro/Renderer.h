#pragma once

class Engine;

class Renderer
{
public:
	static Renderer* CreateRenderer(Engine* engine);

	~Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	void OnInit();
	void OnDestroy();
	void OnRender();

private:
	Renderer(Engine* engine);

	void LoadPipeline();
	void LoadAssets();
	void MoveToNextFrame();
	void WaitForGpu();

	void BeginRender();
	void EndRender();

private:
	static Renderer* s_Instance;
	static const UINT kFrameCount = 2;

	Engine* m_Engine;

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

	UINT m_RtvDescriptorSize;
	UINT m_FrameIndex;
	HANDLE m_FenceEvent;
	UINT64 m_FenceValues[kFrameCount];
};

