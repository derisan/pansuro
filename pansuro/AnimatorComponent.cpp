#include "pch.h"
#include "AnimatorComponent.h"

#include "Skeleton.h"
#include "Engine.h"
#include "Animation.h"

AnimatorComponent::AnimatorComponent(Skeleton* skel)
	: m_Skeleton(skel)
	, m_UploadBuffer(DEVICE.Get(), 1, true)
	, m_StateMachine(std::make_unique<StateMachine>(this))
{

}

void AnimatorComponent::Bind()
{
	m_UploadBuffer.CopyData(0, m_Palette);

	CMD_LIST->SetGraphicsRootConstantBufferView(RP_BoneTransform, m_UploadBuffer.GetVirtualAddress());
}

void AnimatorComponent::Update(float dt)
{
	if (m_Animation && m_Skeleton)
	{
		m_AnimTime += dt * m_AnimPlayRate;

		while (m_AnimTime > m_Animation->GetDuration())
		{
			m_AnimTime -= m_Animation->GetDuration();
		}

		ComputeMatrixPalette();

		m_StateMachine->Update();
	}
}

float AnimatorComponent::PlayAnimation(Animation* anim, float playRate /*= 1.0f*/)
{
	m_Animation = anim;
	m_AnimTime = 0.0f;
	m_AnimPlayRate = playRate;

	if (!m_Animation)
	{
		return 0.0f;
	}

	ComputeMatrixPalette();

	return m_Animation->GetDuration();
}

void AnimatorComponent::ComputeMatrixPalette()
{
	const std::vector<Matrix>& globalInvBindPoses = m_Skeleton->GetGlobalInvBindPoses();
	std::vector<Matrix> currentPoses;
	m_Animation->GetGlobalPoseAtTime(currentPoses, m_Skeleton, m_AnimTime);

	for (UINT i = 0; i < m_Skeleton->GetNumBones(); i++)
	{
		m_Palette.Entry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}
