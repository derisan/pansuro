#include "pch.h"
#include "AnimatorComponent.h"

#include "Skeleton.h"
#include "Engine.h"
#include "Animation.h"
#include "State.h"

AnimatorComponent::AnimatorComponent(Skeleton* skel)
	: m_Skeleton(skel)
	, m_UploadBuffer(DEVICE.Get(), 1, true)
	, m_StateMachine(std::make_unique<StateMachine>(this))
	, m_bLoop(true)
	, m_ToState(nullptr)
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

		if (m_AnimTime > m_Animation->GetDuration())
		{
			if (m_bLoop)
			{
				m_AnimTime -= m_Animation->GetDuration();
			}
			else 
			{
				if (m_ToState)
				{
					m_StateMachine->ChangeState(m_ToState);
					m_ToState = nullptr;
				}
				else
				{
					m_StateMachine->RevertPreviousState();
				}
			}
		}

		ComputeMatrixPalette();

		m_StateMachine->Update();
	}
}

float AnimatorComponent::PlayAnimation(Animation* anim, bool bLoop, State* toState, float playRate)
{
	if (m_Animation == anim)
	{
		return 0.0f;
	}

	m_Animation = anim;
	m_AnimTime = 0.0f;

	m_bLoop = bLoop;
	m_ToState = toState;
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
