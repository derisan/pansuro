#include "pch.h"
#include "State.h"

#include "StateMachine.h"
#include "ResourceManager.h"
#include "Input.h"


IdleState* IdleState::Instance()
{
	static IdleState instance;
	return &instance;
}

void IdleState::Enter(StateMachine* sm)
{
	sm->PlayAnimation(ResourceManager::GetAnimation(L"Assets/Knight_Idle.gpanim"));
}

void IdleState::Update(StateMachine* sm)
{

}

void IdleState::Exit(StateMachine* sm)
{

}

RunState* RunState::Instance()
{
	static RunState instance;
	return &instance;
}

void RunState::Enter(StateMachine* sm)
{
	sm->PlayAnimation(ResourceManager::GetAnimation(L"Assets/Knight_Run.gpanim"));
}

void RunState::Update(StateMachine* sm)
{

}

void RunState::Exit(StateMachine* sm)
{

}
