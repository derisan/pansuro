#include "pch.h"
#include "ResourceManager.h"

std::unordered_map<std::wstring, IResource*> ResourceManager::m_Resources;
