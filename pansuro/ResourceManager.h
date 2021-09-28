#pragma once

#include "Core.h"


#include "Mesh.h"
#include "Texture.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Resource.h"

class IResource;

class ResourceManager
{
public:
	static Mesh* GetMesh(const std::wstring& path)
	{
		IResource* mesh = GetResource<Mesh>(path);

		if (mesh)
		{
			return (Mesh*)mesh;
		}

		else
		{
			MK_CRITICAL("Mesh file not found: {0}", ws2s(path));
			return nullptr;
		}
	}

	static Texture* GetTexture(const std::wstring& path)
	{
		IResource* texture = GetResource<Texture>(path);

		if (texture)
		{
			return (Texture*)texture;
		}

		else
		{
			MK_CRITICAL("Texture file not found: {0}", ws2s(path));
			return nullptr;
		}
	}

	static Skeleton* GetSkeleton(const std::wstring& path)
	{
		IResource* skel = GetResource<Skeleton>(path);

		if (skel)
		{
			return (Skeleton*)skel;
		}
		else
		{
			MK_CRITICAL("Skeleton file not found: {0}", ws2s(path));
			return nullptr;
		}
	}

	static Animation* GetAnimation(const std::wstring& path)
	{
		IResource* anim = GetResource<Animation>(path);

		if (anim)
		{
			return (Animation*)anim;
		}
		else
		{
			MK_CRITICAL("Animation file not found: {0}", ws2s(path));
			return nullptr;
		}
	}

private:
	template<typename T>
	static IResource* GetResource(const std::wstring& path)
	{
		auto iter = m_Resources.find(path);

		if (iter != m_Resources.end())
		{
			return iter->second;
		}
		
		else
		{
			IResource* res = new T();
			res->Load(path);
			return res;
		}
	}

private:
	static std::unordered_map<std::wstring, IResource*> m_Resources;
};