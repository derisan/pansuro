#pragma once

#include "Core.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices,
		const std::vector<UINT>& indices);

	D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() { return &m_VertexBufferView; }
	D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() { return &m_IndexBufferView; }

	UINT GetIndexCount() const { return m_IndexCount; }

	static Mesh* CreateMesh(const std::wstring& path);

private:
	void CreateVertexBuffer(const std::vector<Vertex>& vertices);
	void CreateIndexBuffer(const std::vector<UINT>& indices);

private:
	ComPtr<ID3D12Resource> m_VertexBuffer;
	ComPtr<ID3D12Resource> m_VertexUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	UINT m_VertexCount;

	ComPtr<ID3D12Resource> m_IndexBuffer;
	ComPtr<ID3D12Resource> m_IndexUploadBuffer;
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
	UINT m_IndexCount;
};