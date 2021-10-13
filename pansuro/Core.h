#pragma once

#include "d3dHelper.h"
#include "Log.h"
#include "Types.h"

#ifdef MK_DEBUG
#define MK_ASSERT(x, ...) {if(!(x)) { MK_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define MK_ASSERT(x, ...) 
#endif // MK_DEBUG

inline std::wstring s2ws(const std::string& s)
{
	UINT len;
	UINT slength = static_cast<UINT>(s.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring ret(buf);
	delete[] buf;
	return ret;
}

inline std::string ws2s(const std::wstring& s)
{
	UINT len;
	UINT slength = static_cast<UINT>(s.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}