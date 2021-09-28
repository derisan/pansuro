#pragma once

#include "Core.h"
#include "Resource.h"

class Texture : public IResource
{
public:
	virtual void Load(const std::wstring& path) override;

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return m_SrvGpuHandle; }

private:
	static UINT s_NumTextures;

	ScratchImage m_RawImage;
	ComPtr<ID3D12Resource> m_Texture;
	D3D12_GPU_DESCRIPTOR_HANDLE m_SrvGpuHandle;
	UINT m_MyIndex;
};