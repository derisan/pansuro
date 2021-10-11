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
		, m_CurrentSpeed(0.0f)
		, transform(GetComponent<TransformComponent>())
		, animator(GetComponent<AnimatorComponent>())
	{}

	virtual void OnCreate() override
	{
		animator.GetStateMachine()->SetScript(this);
	}

	virtual void OnUpdate(float dt) override
	{
		m_CurrentSpeed = 0.0f;

		if (INPUT->IsButtonHold(KeyType::W))
		{
			m_CurrentSpeed = m_Speed;
			transform.RotateYaw(0.0f);
		}

		else if (INPUT->IsButtonHold(KeyType::A))
		{
			m_CurrentSpeed = m_Speed;
			transform.RotateYaw(-90.0f);
		}

		else if (INPUT->IsButtonHold(KeyType::S))
		{
			m_CurrentSpeed = m_Speed;
			transform.RotateYaw(180.0f);
		}

		else if (INPUT->IsButtonHold(KeyType::D))
		{
			m_CurrentSpeed = m_Speed;
			transform.RotateYaw(90.0f);
		}

		transform.MoveForward(m_CurrentSpeed * dt);
	}

	float GetCurrentSpeed() const
	{
		return m_CurrentSpeed;
	}

	virtual void OnDestroy() override
	{

	}

private:
	const float m_Speed;
	float m_CurrentSpeed;

	TransformComponent& transform;
	AnimatorComponent& animator;
};