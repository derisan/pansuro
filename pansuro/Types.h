#pragma once

#include "pch.h"

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

	void Rotate(const Vector3& rotation)
	{
		std::array<Vector3, 8> points;

		points[0] = Min;
		points[1] = Vector3(Max.x, Min.y, Min.z);
		points[2] = Vector3(Min.x, Max.y, Min.z);
		points[3] = Vector3(Min.x, Min.y, Max.z);
		points[4] = Vector3(Min.x, Max.y, Max.z);
		points[5] = Vector3(Max.x, Min.y, Max.z);
		points[6] = Vector3(Max.x, Max.y, Min.z);
		points[7] = Vector3(Max);

		Quaternion q = Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.z));
		Vector3 p = Vector3::Transform(points[0], q);
		Min = p;
		Max = p;

		for (auto i = 1; i < points.size(); i++)
		{
			p = Vector3::Transform(points[i], q);
			UpdateMinMax(p);
		}
	}

	Vector3 Min = { FLT_MAX, FLT_MAX, FLT_MAX };
	Vector3 Max = { FLT_MIN, FLT_MIN, FLT_MIN };
	BoundingBox Box = {};
};

enum RootParam
{
	RP_World,
	RP_ViewProj,
	RP_Texture,
	RP_BoneTransform,
};