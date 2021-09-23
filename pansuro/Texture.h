#pragma once

#include "Core.h"

class Texture
{
public:
	static Texture* CreateTexture(const std::wstring& path);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return m_SrvGpuHandle; }

private:
	Texture();
	bool LoadTexture(const std::wstring& path);

private:
	static UINT s_NumTextures;

	ScratchImage m_RawImage;
	ComPtr<ID3D12Resource> m_Texture;
	D3D12_GPU_DESCRIPTOR_HANDLE m_SrvGpuHandle;
	UINT m_MyIndex;
};