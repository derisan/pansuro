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

constexpr unsigned int MAX_SKELETON_BONES = 96;

struct SkinnedVertex
{
	Vector3 Position;
	Vector3 Normal;
	UINT Bone[4];
	Vector4 Weight;
	Vector2 UV;
};

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 UV;
};

struct BoneTransform
{
	Vector3 Translation;
	Quaternion Rotation;
	
	Matrix ToMatrix() const
	{
		Matrix mat = Matrix::CreateFromQuaternion(Rotation);
		mat *= Matrix::CreateTranslation(Translation); 

		return mat;
	}

	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
	{
		BoneTransform ret;
		ret.Rotation = Quaternion::Slerp(a.Rotation, b.Rotation, f);
		ret.Translation = Vector3::Lerp(a.Translation, b.Translation, f);

		return ret;
	}
};

struct Bone
{
	BoneTransform LocalBindPose;
	std::string Name;
	int Parent;
};

struct MatrixPalette
{
	Matrix Entry[MAX_SKELETON_BONES];
};

struct AABB
{
	AABB() = default;

	void UpdateMinMax(const Vector3& point)
	{
		Min.x = min(Min.x, point.x);
		Min.y = min(Min.y, point.y);
		Min.z = min(Min.z, point.z);

		Max.x = max(Max.x, point.x);
		Max.y = max(Max.y, point.y);
		Max.z = max(Max.z, point.z);
	}

	void GenerateBox()
	{
		BoundingBox::CreateFromPoints(Box, Min, Max);
	}

	Vector3 Min = { FLT_MAX, FLT_MAX, FLT_MAX };
	Vector3 Max = { FLT_MIN, FLT_MIN, FLT_MIN };
	BoundingBox Box = {};
};

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