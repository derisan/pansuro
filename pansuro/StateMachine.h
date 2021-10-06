#pragma once

#include "Core.h"

class AnimatorComponent;
class State;
class Animation;

class StateMachine
{
public:
	StateMachine(AnimatorComponent* animator);
	~StateMachine();

	void Update();

	void ChangeState(State* newState);
	void PlayAnimation(Animation* animation);

private:
	State* m_CurrentState;
	State* m_PreviousState;

	AnimatorComponent* m_Animator;
};

