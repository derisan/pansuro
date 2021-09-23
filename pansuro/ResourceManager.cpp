#include "pch.h"
#include "ResourceManager.h"

#include "Mesh.h"
#include "Texture.h"

std::unordered_map<std::wstring, Mesh*> ResourceManager::m_Meshes;
std::unordered_map<std::wstring, Texture*> ResourceManager::m_Textures;

Mesh* ResourceManager::LoadMesh(const std::wstring& path)
{
	auto iter = m_Meshes.find(path);
	if (iter != m_Meshes.end())
	{
		return iter->second;
	}
	else
	{
		auto mesh = Mesh::CreateMesh(path);
		if (!mesh)
		{
			m_Meshes[path] = mesh;
		}
		return mesh;
	}
}

Mesh* ResourceManager::LoadCubeMesh()
{
	auto iter = m_Meshes.find(L"Cube");
	if (iter != m_Meshes.end())
	{
		return iter->second;
	}

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	std::vector<Vertex> vertices(24);

	// ¾Õ¸é
	vertices[0] = Vertex{ Vector3(-w2, -h2, -d2), Vector2(0.0f, 1.0f) };
	vertices[1] = Vertex{ Vector3(-w2, +h2, -d2), Vector2(0.0f, 0.0f) };
	vertices[2] = Vertex{ Vector3(+w2, +h2, -d2), Vector2(1.0f, 0.0f) };
	vertices[3] = Vertex{ Vector3(+w2, -h2, -d2), Vector2(1.0f, 1.0f) };
	// µÞ¸é		 											   
	vertices[4] = Vertex{ Vector3(-w2, -h2, +d2), Vector2(1.0f, 1.0f) };
	vertices[5] = Vertex{ Vector3(+w2, -h2, +d2), Vector2(0.0f, 1.0f) };
	vertices[6] = Vertex{ Vector3(+w2, +h2, +d2), Vector2(0.0f, 0.0f) };
	vertices[7] = Vertex{ Vector3(-w2, +h2, +d2), Vector2(1.0f, 0.0f) };
	// À­¸é
	vertices[8] = Vertex{ Vector3(-w2, +h2, -d2), Vector2(0.0f, 1.0f) };
	vertices[9] = Vertex{ Vector3(-w2, +h2, +d2), Vector2(0.0f, 0.0f) };
	vertices[10] = Vertex{ Vector3(+w2, +h2, +d2), Vector2(1.0f, 0.0f) };
	vertices[11] = Vertex{ Vector3(+w2, +h2, -d2), Vector2(1.0f, 1.0f) };
	// ¾Æ·§¸é
	vertices[12] = Vertex{ Vector3(-w2, -h2, -d2), Vector2(1.0f, 1.0f) };
	vertices[13] = Vertex{ Vector3(+w2, -h2, -d2), Vector2(0.0f, 1.0f) };
	vertices[14] = Vertex{ Vector3(+w2, -h2, +d2), Vector2(0.0f, 0.0f) };
	vertices[15] = Vertex{ Vector3(-w2, -h2, +d2), Vector2(1.0f, 0.0f) };
	// ¿ÞÂÊ¸é		 									
	vertices[16] = Vertex{ Vector3(-w2, -h2, +d2), Vector2(0.0f, 1.0f) };
	vertices[17] = Vertex{ Vector3(-w2, +h2, +d2), Vector2(0.0f, 0.0f) };
	vertices[18] = Vertex{ Vector3(-w2, +h2, -d2), Vector2(1.0f, 0.0f) };
	vertices[19] = Vertex{ Vector3(-w2, -h2, -d2), Vector2(1.0f, 1.0f) };
	// ¿À¸¥ÂÊ¸é
	vertices[20] = Vertex{ Vector3(+w2, -h2, -d2), Vector2(0.0f, 1.0f) };
	vertices[21] = Vertex{ Vector3(+w2, +h2, -d2), Vector2(0.0f, 0.0f) };
	vertices[22] = Vertex{ Vector3(+w2, +h2, +d2), Vector2(1.0f, 0.0f) };
	vertices[23] = Vertex{ Vector3(+w2, -h2, +d2), Vector2(1.0f, 1.0f) };

	std::vector<UINT> indices(36);

	// ¾Õ¸é
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	// µÞ¸é
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;
	// À­¸é
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;
	// ¾Æ·§¸é
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;
	// ¿ÞÂÊ¸é
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;
	// ¿À¸¥ÂÊ¸é
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	auto mesh = new Mesh(vertices, indices);
	m_Meshes[L"Cube"] = mesh;

	return mesh;
}

Texture* ResourceManager::LoadTexture(const std::wstring& path)
{
	auto iter = m_Textures.find(path);
	if (iter != m_Textures.end())
	{
		return iter->second;
	}
	else
	{
		auto tex = Texture::CreateTexture(path);
		if (tex)
		{
			m_Textures[path] = tex;
			MK_INFO("Num textures: {0}", m_Textures.size());
		}
		return tex;
	}
}
