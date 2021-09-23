#pragma once

#include "Core.h"

class TextureDescriptorHeap
{
public:
	TextureDescriptorHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(int index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(int index);

	ComPtr<ID3D12DescriptorHeap> GetHeap() { return m_DescriptorHeap; }

private:
	UINT m_SrvDescriptorSize;

	ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;
};