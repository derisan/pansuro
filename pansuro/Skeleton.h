#pragma once

#include "Core.h"
#include "Resource.h"

class Skeleton : public IResource
{
public:
	virtual void Load(const std::wstring& path) override;

	UINT GetNumBones() const { return static_cast<UINT>(m_Bones.size()); }
	const Bone& GetBone(UINT idx) const { return m_Bones[idx]; }
	const std::vector<Bone>& GetBones() const { return m_Bones; }
	const std::vector<Matrix>& GetGlobalInvBindPoses() const { return m_GlobalInvBindPoses; }

private:
	void ComputeGlobalInvBindPose();

private:
	std::vector<Bone> m_Bones;
	std::vector<Matrix> m_GlobalInvBindPoses;
};

