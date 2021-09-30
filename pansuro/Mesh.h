#pragma once

#include "Core.h"
#include "Resource.h"

#include <rapidjson/document.h>

class Mesh : public IResource
{
public:
	D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() { return &m_VertexBufferView; }
	D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() { return &m_IndexBufferView; }

	UINT GetIndexCount() const { return m_IndexCount; }

	virtual void Load(const std::wstring& path) override;

	const AABB& GetAABB() const { return m_AABB; }

	void CreateDebugMesh(const Vector3& minPoint, const Vector3& maxPoint);

private:
	void LoadStaticMesh(const rapidjson::Document& doc);
	void LoadSkeletalMesh(const rapidjson::Document& doc);

	void CreateSkinnedVertexBuffer(const std::vector<SkinnedVertex>& vertices);
	void CreateVertexBuffer(const std::vector<Vertex>& vertices);
	void CreateIndexBuffer(const std::vector<UINT>& indices);

private:
	ComPtr<ID3D12Resource> m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	UINT m_VertexCount;

	ComPtr<ID3D12Resource> m_IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
	UINT m_IndexCount;

	AABB m_AABB = {};
};