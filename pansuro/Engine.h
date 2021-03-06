#pragma once

#include "Core.h"

class Mesh;
class Scene;
class TextureDescriptorHeap;
class PipelineState;


class Engine
{
public:
	~Engine() = default;

	static Engine* CreateEngine(UINT width, UINT height, std::wstring title);

	void Init();
	void Shutdown();
	void Update();
	void Render();

	UINT GetWidth() const { return m_Width; }
	UINT GetHeight() const { return m_Height; }
	float GetAspectRatio() const { return m_AspectRatio; }
	const std::wstring& GetTitle() const { return m_Title; }

	ComPtr<ID3D12Device> GetDevice() { return m_Device; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return m_CmdList; }
	void AddUsedUploadBuffer(ComPtr<ID3D12Resource> buffer) { m_UsedUploadBuffers.push_back(buffer); }

	static Engine* GetEngine() { return s_Instance; }
	std::unique_ptr<TextureDescriptorHeap>& GetTexHeap() { return m_TextureDescriptorHeap; }

	ComPtr<ID3D12PipelineState> GetSkinnedPSO();
	ComPtr<ID3D12PipelineState> GetDefaultPSO();
	ComPtr<ID3D12PipelineState> GetDebugPSO();

private:
	Engine(UINT width, UINT height, std::wstring title);

	void LoadPipeline();
	void LoadAssets();
	void CreateRootSignature();

	void WaitForPreviousFrame();

	void BeginRender();
	void EndRender();

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
	ComPtr<ID3D12CommandAllocator> m_CmdAllocator;
	ComPtr<ID3D12CommandQueue> m_CmdQueue;
	ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	ComPtr<ID3D12GraphicsCommandList> m_CmdList;
	ComPtr<ID3D12Fence> m_Fence;
	ComPtr<ID3D12RootSignature> m_RootSignature;
	ComPtr<ID3D12Resource> m_DsvBuffer;
	ComPtr<ID3D12DescriptorHeap> m_DsvHeap;

	UINT m_RtvDescriptorSize;
	UINT m_FrameIndex;
	HANDLE m_FenceEvent;
	UINT64 m_FenceValue;

	std::unique_ptr<PipelineState> m_SkinnedPSO;
	std::unique_ptr<PipelineState> m_DefaultPSO;
	std::unique_ptr<PipelineState> m_DebugPSO;

	std::unique_ptr<TextureDescriptorHeap> m_TextureDescriptorHeap;
	std::unique_ptr<Scene> m_ActiveScene;

	std::vector<ComPtr<ID3D12Resource>> m_UsedUploadBuffers;
};

#define ENGINE Engine::GetEngine()
#define DEVICE Engine::GetEngine()->GetDevice()
#define CMD_LIST Engine::GetEngine()->GetCmdList()
#define RELEASE_UPLOAD_BUFFER(x) Engine::GetEngine()->AddUsedUploadBuffer(x)
#define TEXHEAP Engine::GetEngine()->GetTexHeap()