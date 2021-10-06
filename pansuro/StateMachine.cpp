#include "pch.h"
#include "StateMachine.h"

#include "AnimatorComponent.h"
#include "State.h"

StateMachine::StateMachine(AnimatorComponent* animator)
	: m_Animator(animator)
	, m_CurrentState(IdleState::Instance())
	, m_PreviousState(nullptr)
{
	m_CurrentState->Enter(this);
}

StateMachine::~StateMachine()
{

}

void StateMachine::Update()
{
	m_CurrentState->Update(this);
}

void StateMachine::ChangeState(State* newState)
{
	m_PreviousState = m_CurrentState;
	m_CurrentState->Exit(this);
	m_CurrentState = newState;
	m_CurrentState->Enter(this);
}

void StateMachine::PlayAnimation(Animation* animation)
{
	m_Animator->PlayAnimation(animation);
}
