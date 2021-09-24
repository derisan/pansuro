#pragma once

#include "pch.h"
#include "d3dHelper.h"
#include "Log.h"

#define BIT(x) ((1) << (x))

#ifdef MK_DEBUG
#define MK_ASSERT(x, ...) {if(!(x)) { MK_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define MK_ASSERT(x, ...) 
#endif // MK_DEBUG

struct Vertex
{
	Vector3 Position;
	Vector2 UV;
};