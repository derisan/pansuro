#include "pch.h"
#include "State.h"

#include "StateMachine.h"
#include "ResourceManager.h"
#include "Input.h"
#include "CharacterMovement.h"

IdleState* IdleState::Instance()
{
	static IdleState instance;
	return &instance;
}

void IdleState::Enter(StateMachine* sm)
{
	sm->PlayAnimation(ResourceManager::GetAnimation(L"Assets/Knight_Idle.gpanim"));
}

void IdleState::Update(StateMachine* sm, Script* sc)
{
	auto cm = (CharacterMovement*)sc;

	if (INPUT->IsButtonDown(KeyType::LBUTTON))
	{
		sm->ChangeState(PunchState::Instance());
	}

	else if (cm->GetCurrentSpeed() > 0.0f)
	{
		sm->ChangeState(RunState::Instance());
	}
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

void RunState::Update(StateMachine* sm, Script* sc)
{
	auto cm = (CharacterMovement*)sc;

	if (INPUT->IsButtonDown(KeyType::LBUTTON))
	{
		sm->ChangeState(PunchState::Instance());
	}

	else if (cm->GetCurrentSpeed() == 0.0f)
	{
		sm->ChangeState(IdleState::Instance());
	}
}

void RunState::Exit(StateMachine* sm)
{

}

PunchState* PunchState::Instance()
{
	static PunchState instance;
	return &instance;
}

void PunchState::Enter(StateMachine* sm)
{
	sm->PlayAnimation(ResourceManager::GetAnimation(L"Assets/Knight_Punch.gpanim"), false, IdleState::Instance(), 2.0f);
}

void PunchState::Update(StateMachine* sm, Script* sc)
{
	auto cm = (CharacterMovement*)sc;

	if (cm->GetCurrentSpeed() > 0.0f)
	{
		sm->ChangeState(RunState::Instance());
	}
}

void PunchState::Exit(StateMachine* sm)
{

}
