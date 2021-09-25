#pragma once

#include "Core.h"

class Skeleton
{
public:
	static Skeleton* Load(const std::wstring& path);

	UINT GetNumBones() const { return m_Bones.size(); }
	const Bone& GetBone(UINT idx) const { return m_Bones[idx]; }
	const std::vector<Bone>& GetBones() const { return m_Bones; }
	const std::vector<Matrix> GetGlobalInvBindPoses() const { return m_GlobalInvBindPoses; }

private:
	Skeleton() = default;
	void ComputeGlobalInvBindPose();

private:
	std::vector<Bone> m_Bones;
	std::vector<Matrix> m_GlobalInvBindPoses;
};

