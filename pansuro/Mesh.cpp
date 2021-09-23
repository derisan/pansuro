#include "pch.h"
#include "Mesh.h"

#include "Engine.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
{
	CreateVertexBuffer(vertices);
	CreateIndexBuffer(indices);
}

Mesh* Mesh::CreateMesh(const std::wstring& path)
{
	return nullptr;
}

void Mesh::CreateVertexBuffer(const std::vector<Vertex>& vertices)
{
	m_VertexCount = static_cast<UINT>(vertices.size());
	UINT bufferSize = m_VertexCount * sizeof(Vertex);
	
	ComPtr<ID3D12Resource> vertexUploadBuffer;
	const CD3DX12_HEAP_PROPERTIES uploadBufferHeapProps(D3D12_HEAP_TYPE_UPLOAD);
	const CD3DX12_RESOURCE_DESC uploadbufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	DEVICE->CreateCommittedResource(
		&uploadBufferHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&uploadbufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexUploadBuffer));

	void* mappedData;
	vertexUploadBuffer->Map(0, nullptr, &mappedData);
	::memcpy(mappedData, vertices.data(), bufferSize);
	vertexUploadBuffer->Unmap(0, nullptr);

	const CD3DX12_HEAP_PROPERTIES defaultBufferHeapProps(D3D12_HEAP_TYPE_DEFAULT);
	const CD3DX12_RESOURCE_DESC defaultbufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	DEVICE->CreateCommittedResource(
		&defaultBufferHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&defaultbufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_VertexBuffer));

	const auto toDefaultBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	CMD_LIST->CopyResource(m_VertexBuffer.Get(), vertexUploadBuffer.Get());
	CMD_LIST->ResourceBarrier(1, &toDefaultBarrier);
	
	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.SizeInBytes = bufferSize;
	m_VertexBufferView.StrideInBytes = sizeof(Vertex);

	RELEASE_UPLOAD_BUFFER(vertexUploadBuffer);
}

void Mesh::CreateIndexBuffer(const std::vector<UINT>& indices)
{
	m_IndexCount = static_cast<UINT>(indices.size());
	UINT bufferSize = sizeof(UINT) * m_IndexCount;

	ComPtr<ID3D12Resource> indexUploadBuffer;
	const CD3DX12_HEAP_PROPERTIES uploadBufferHeapProps(D3D12_HEAP_TYPE_UPLOAD);
	const CD3DX12_RESOURCE_DESC uploadbufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	DEVICE->CreateCommittedResource(
		&uploadBufferHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&uploadbufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&indexUploadBuffer));

	void* mappedData;
	indexUploadBuffer->Map(0, nullptr, &mappedData);
	::memcpy(mappedData, indices.data(), bufferSize);
	indexUploadBuffer->Unmap(0, nullptr);

	const CD3DX12_HEAP_PROPERTIES defaultBufferHeapProps(D3D12_HEAP_TYPE_DEFAULT);
	const CD3DX12_RESOURCE_DESC defaultbufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	DEVICE->CreateCommittedResource(
		&defaultBufferHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&defaultbufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_IndexBuffer));

	const auto toDefaultBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
	CMD_LIST->CopyResource(m_IndexBuffer.Get(), indexUploadBuffer.Get());
	CMD_LIST->ResourceBarrier(1, &toDefaultBarrier);

	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufferView.SizeInBytes = bufferSize;

	RELEASE_UPLOAD_BUFFER(indexUploadBuffer);
}
