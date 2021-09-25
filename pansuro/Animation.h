#pragma once

#include "Core.h"

class Skeleton;

class Animation
{
public:
	static Animation* Load(const std::wstring& path);

	UINT GetNumBones() const { return m_NumBones; }
	UINT GetNumFrames() const { return m_NumFrames; }
	float GetDuration() const { return m_Duration; }
	float GetFrameDuration() const { return m_FrameDuration; }

	void GetGlobalPoseAtTime(std::vector<Matrix>& outPoses, const Skeleton* inSkeleton, float inTime) const;

private:
	UINT m_NumBones;
	UINT m_NumFrames;
	float m_Duration;
	float m_FrameDuration;

	std::vector<std::vector<BoneTransform>> m_Tracks;
};

