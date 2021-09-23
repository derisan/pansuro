#pragma once

#include "Log.h"
#include "d3dHelper.h"

#ifdef _DEBUG
#define MK_ASSERT(x, ...) {if(!(x)) { MK_CRITICAL("Assertion Failed: ", __VA_ARGS__); __debugbreak();}}
#else
#define MK_ASSERT(x, ...) 
#endif
