#include "pch.h"
#include "TextureDescriptorHeap.h"

#include "Engine.h"

TextureDescriptorHeap::TextureDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 100;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	m_SrvDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_DescriptorHeap));
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureDescriptorHeap::GetCpuHandle(int index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(), index * m_SrvDescriptorSize);
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureDescriptorHeap::GetGpuHandle(int index)
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(), index * m_SrvDescriptorSize);
}
