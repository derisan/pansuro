#pragma once

#include "Core.h"
#include "Script.h"

#include "TransformComponent.h"
#include "AnimatorComponent.h"
#include "ResourceManager.h"

#include "Input.h"

class CharacterMovement : public Script
{
public:
	CharacterMovement(Entity* owner, float speed = 50.0f)
		: Script(owner)
		, m_Speed(speed)
		, transform(GetComponent<TransformComponent>())
	{}

	virtual void OnCreate() override
	{
		
	}

	virtual void OnUpdate(float dt) override
	{
		if (INPUT->IsButtonHold(KeyType::W))
		{
			transform.RotateYaw(0.0f);
			transform.MoveForward(m_Speed * dt);
			
		}

		if (INPUT->IsButtonHold(KeyType::A))
		{
			transform.RotateYaw(-90.0f);
			transform.MoveForward(m_Speed * dt);
		}

		if (INPUT->IsButtonHold(KeyType::S))
		{
			transform.RotateYaw(180.0f);
			transform.MoveForward(m_Speed * dt);
		}

		if (INPUT->IsButtonHold(KeyType::D))
		{
			transform.RotateYaw(90.0f);
			transform.MoveForward(m_Speed * dt);
		}
	}

	virtual void OnDestroy() override
	{

	}

private:
	float m_Speed;

	TransformComponent& transform;
};