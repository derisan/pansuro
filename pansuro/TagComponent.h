#pragma once

#include "Core.h"

class TagComponent
{
public:
	TagComponent(const std::wstring& tag = std::wstring());

	bool operator==(const TagComponent& other) const
	{
		return m_Tag == other.m_Tag;
	}

	bool operator==(const std::wstring& tag) const
	{
		return m_Tag == tag;
	}

	bool operator!=(const TagComponent& other) const
	{
		return !(*this == other);
	}

private:
	std::wstring m_Tag;
};

