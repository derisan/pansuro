#include "pch.h"
#include "StateMachine.h"

#include "AnimatorComponent.h"
#include "State.h"
#include "Script.h"

StateMachine::StateMachine(AnimatorComponent* animator)
	: m_Animator(animator)
	, m_CurrentState(IdleState::Instance())
	, m_PreviousState(m_CurrentState)
	, m_Script(nullptr)
{
	m_CurrentState->Enter(this);
}

StateMachine::~StateMachine()
{

}

void StateMachine::Update()
{
	if (m_Script)
	{
		m_CurrentState->Update(this, m_Script);
	}
}

void StateMachine::ChangeState(State* newState)
{
	m_PreviousState = m_CurrentState;
	m_CurrentState->Exit(this);
	m_CurrentState = newState;
	m_CurrentState->Enter(this);
}

void StateMachine::RevertPreviousState()
{
	ChangeState(m_PreviousState);
}

void StateMachine::PlayAnimation(Animation* animation, bool bLoop, State* toState, float playRate)
{
	m_Animator->PlayAnimation(animation, bLoop, toState, playRate);
}
