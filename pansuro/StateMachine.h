#pragma once

#include "Core.h"

class AnimatorComponent;
class State;
class Animation;
class Script;

class StateMachine
{
public:
	StateMachine(AnimatorComponent* animator);
	~StateMachine();

	void Update();

	void ChangeState(State* newState);
	void RevertPreviousState();
	void PlayAnimation(Animation* animation, bool bLoop = true, State* toState = nullptr, float playRate = 1.0f);

	void SetScript(Script* script) { m_Script = script; }

private:
	State* m_CurrentState;
	State* m_PreviousState;

	AnimatorComponent* m_Animator;
	Script* m_Script;
};

