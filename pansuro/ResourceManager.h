#pragma once

#include "Core.h"

class Mesh;
class Texture;

class ResourceManager
{
public:
	static Mesh* GetMesh(const std::wstring& path);
	static Texture* GetTexture(const std::wstring& path);

private:
	static std::unordered_map<std::wstring, Mesh*> m_Meshes;
	static std::unordered_map<std::wstring, Texture*> m_Textures;
};