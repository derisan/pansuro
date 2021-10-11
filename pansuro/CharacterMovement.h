#pragma once

#include "Core.h"
#include "Script.h"

#include "TransformComponent.h"
#include "AnimatorComponent.h"
#include "ResourceManager.h"
#include "State.h"

#include "Input.h"

class CharacterMovement : public Script
{
public:
	CharacterMovement(Entity* owner, float speed = 50.0f)
		: Script(owner)
		, m_Speed(speed)
		, transform(GetComponent<TransformComponent>())
		, animator(GetComponent<AnimatorComponent>())
	{}

	virtual void OnCreate() override
	{
		
	}

	virtual void OnUpdate(float dt) override
	{
		float speed = 0.0f;

		if (INPUT->IsButtonHold(KeyType::W))
		{
			speed = m_Speed;
			transform.RotateYaw(0.0f);
		}

		else if (INPUT->IsButtonHold(KeyType::A))
		{
			speed = m_Speed;
			transform.RotateYaw(-90.0f);
		}

		else if (INPUT->IsButtonHold(KeyType::S))
		{
			speed = m_Speed;
			transform.RotateYaw(180.0f);
		}

		else if (INPUT->IsButtonHold(KeyType::D))
		{
			speed = m_Speed;
			transform.RotateYaw(90.0f);
		}

		transform.MoveForward(speed * dt);

		if (speed != 0.0f)
		{
			animator.GetStateMachine()->ChangeState(RunState::Instance());
		}
		else
		{
			animator.GetStateMachine()->ChangeState(IdleState::Instance());
		}
	}

	virtual void OnDestroy() override
	{

	}

private:
	float m_Speed;

	TransformComponent& transform;
	AnimatorComponent& animator;
};