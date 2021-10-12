#pragma once

#include "Core.h"
#include "StateMachine.h"

class Skeleton;
class Animation;
class State;

class AnimatorComponent
{
public:
	AnimatorComponent(Skeleton* skel);

	void Bind();
	void Update(float dt);

	float PlayAnimation(Animation* anim, bool bLoop = true, State* toState = nullptr, float playRate = 1.0f);

	const std::unique_ptr<StateMachine>& GetStateMachine() const { return m_StateMachine; }

private:
	void ComputeMatrixPalette();

private:
	MatrixPalette m_Palette;

	Skeleton* m_Skeleton;
	Animation* m_Animation;

	float m_AnimPlayRate;
	float m_AnimTime;
	bool m_bLoop;
	State* m_ToState;

	UploadBuffer<MatrixPalette> m_UploadBuffer;

	std::unique_ptr<StateMachine> m_StateMachine;
};

