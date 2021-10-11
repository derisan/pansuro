#pragma once

#include "Core.h"

class StateMachine;

class State
{
public:
	virtual ~State() = default;

	virtual void Enter(StateMachine*) = 0;
	virtual void Update(StateMachine*) = 0;
	virtual void Exit(StateMachine*) = 0;
};

class IdleState : public State
{
public:
	IdleState(const IdleState&) = delete;
	IdleState& operator=(const IdleState&) = delete;

	static IdleState* Instance();

	virtual void Enter(StateMachine* sm) override;

	virtual void Update(StateMachine* sm) override;

	virtual void Exit(StateMachine* sm) override;
	
private:
	IdleState() = default;
};

class RunState : public State
{
public:
	RunState(const RunState&) = delete;
	RunState& operator=(const RunState&) = delete;

	static RunState* Instance();

	virtual void Enter(StateMachine* sm) override;

	virtual void Update(StateMachine* sm) override;

	virtual void Exit(StateMachine* sm) override;

private:
	RunState() = default;
};

class PunchState : public State
{
public:
	PunchState(const PunchState&) = delete;
	PunchState& operator=(const PunchState&) = delete;

	static PunchState* Instance();

	virtual void Enter(StateMachine* sm) override;

	virtual void Update(StateMachine* sm) override;

	virtual void Exit(StateMachine* sm) override;

private:
	PunchState() = default;
};