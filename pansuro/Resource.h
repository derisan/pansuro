#pragma once

#include "Core.h"

#include <string>

class IResource
{
public:
	virtual ~IResource() = default;
	virtual void Load(const std::wstring& path) = 0;
};

