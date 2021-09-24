#pragma once

#include "Core.h"

#include <xhash>

class MyUUID
{
public:
	MyUUID();

	operator UINT64() { return m_UUID; }
	operator const UINT64() const { return m_UUID; }

private:
	UINT64 m_UUID;
};

namespace std {

	template<>
	struct hash<MyUUID>
	{
		std::size_t operator()(const MyUUID& uuid) const
		{
			return hash<UINT64>()((UINT64)uuid);
		}
	};

}