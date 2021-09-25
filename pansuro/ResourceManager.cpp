#include "pch.h"
#include "ResourceManager.h"

#include "Mesh.h"
#include "Texture.h"
#include "Skeleton.h"

std::unordered_map<std::wstring, Mesh*> ResourceManager::m_Meshes;
std::unordered_map<std::wstring, Texture*> ResourceManager::m_Textures;
std::unordered_map<std::wstring, Skeleton*> ResourceManager::m_Skeletons;

Mesh* ResourceManager::GetMesh(const std::wstring& path)
{
	auto iter = m_Meshes.find(path);
	if (iter != m_Meshes.end())
	{
		return iter->second;
	}
	else
	{
		auto mesh = Mesh::Load(path);
		if (mesh)
		{
			m_Meshes[path] = mesh;
		}
		return mesh;
	}
}

Texture* ResourceManager::GetTexture(const std::wstring& path)
{
	auto iter = m_Textures.find(path);
	if (iter != m_Textures.end())
	{
		return iter->second;
	}
	else
	{
		auto tex = Texture::Load(path);
		if (tex)
		{
			m_Textures[path] = tex;
			MK_INFO("Num textures: {0}", m_Textures.size());
		}
		return tex;
	}
}

Skeleton* ResourceManager::GetSkeleton(const std::wstring& path)
{
	auto iter = m_Skeletons.find(path);
	if (iter != m_Skeletons.end())
	{
		return iter->second;
	}
	else
	{
		auto skel = Skeleton::Load(path);
		if (skel)
		{
			m_Skeletons[path] = skel;
		}
		return skel;
	}
}
