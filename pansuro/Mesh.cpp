#include "pch.h"
#include "Mesh.h"

#include "Engine.h"
#include "ResourceManager.h"

#include <rapidjson/document.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
{
	CreateVertexBuffer(vertices);
	CreateIndexBuffer(indices);
}

Mesh* Mesh::Load(const std::wstring& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		MK_ASSERT(nullptr, "Mesh file not found.");
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		MK_ASSERT(nullptr, "It's not valid json file.");
	}

	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		MK_ASSERT(nullptr, "Mesh file has no texture.");
	}

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		std::string texName = textures[i].GetString();
		ResourceManager::GetTexture(s2ws(texName));
	}

	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		MK_ASSERT(nullptr, "Mesh file has no vertices.");
	}

	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size());

	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 16)
		{
			MK_ASSERT(nullptr, "Unknown vertex format.");
		}

		Vertex v;

		v.Position.x = vert[0].GetFloat();
		v.Position.y = vert[2].GetFloat();
		v.Position.z = vert[1].GetFloat();
		v.Normal.x = vert[3].GetFloat();
		v.Normal.y = vert[4].GetFloat();
		v.Normal.z = vert[5].GetFloat();

		v.Bone[0] = vert[6].GetUint();
		v.Bone[1] = vert[7].GetUint();
		v.Bone[2] = vert[8].GetUint();
		v.Bone[3] = vert[9].GetUint();
		
		v.Weight.x = vert[10].GetFloat() / 255.0f;
		v.Weight.y = vert[11].GetFloat() / 255.0f;
		v.Weight.z = vert[12].GetFloat() / 255.0f;
		v.Weight.w = vert[13].GetFloat() / 255.0f;

		v.UV.x = vert[14].GetFloat();
		v.UV.y = vert[15].GetFloat();

		vertices.push_back(std::move(v));
	}

	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		MK_ASSERT(nullptr, "Mesh file has no indices.");
	}

	std::vector<UINT> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			MK_ASSERT(nullptr, "Unknown index format.");
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	return new Mesh(vertices, indices);
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
