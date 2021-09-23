#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             
#endif

#include <windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXTK12/SimpleMath.h>
#include "d3dx12.h"

#include <string>
#include <wrl.h>
#include <shellapi.h>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <array>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace DirectX::PackedVector;
using namespace DirectX::SimpleMath;
