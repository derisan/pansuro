#pragma once

#include "Core.h"

#include "Resource.h"

class Skeleton;

class Animation : public IResource
{
public:
	virtual void Load(const std::wstring& path) override;

	UINT GetNumBones() const { return m_NumBones; }
	UINT GetNumFrames() const { return m_NumFrames; }
	float GetDuration() const { return m_Duration; }
	float GetFrameDuration() const { return m_FrameDuration; }

	void GetGlobalPoseAtTime(std::vector<Matrix>& outPoses, Skeleton* inSkeleton, float inTime) const;

private:
	UINT m_NumBones;
	UINT m_NumFrames;
	float m_Duration;
	float m_FrameDuration;

	std::vector<std::vector<BoneTransform>> m_Tracks;
};

