#pragma once

#include "pch.h"
#include "Log.h"
#include "d3dHelper.h"

#define BIT(x) ((1) << (x))

#ifdef _DEBUG
#define MK_ASSERT(x, ...) {if(!(x)) { MK_CRITICAL("Assertion Failed: ", __VA_ARGS__); __debugbreak();}}
#else
#define MK_ASSERT(x, ...) 
#endif

struct Vertex
{
	Vector3 Position;
	Vector4 Color;
};