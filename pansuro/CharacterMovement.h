#pragma once

#include "Core.h"
#include "Script.h"

#include "TransformComponent.h"

class CharacterMovement : public Script
{
public:
	CharacterMovement(Entity* owner, float speed)
		: Script(owner)
		, m_Speed(speed)
		, transform(GetComponent<TransformComponent>()) {}

	virtual void OnCreate() override
	{
		
	}

	virtual void OnUpdate(float dt) override
	{
		static float rot = 0.0f;

		rot += m_Speed * dt;
		transform.SetRotation(Vector3(0.0f, rot, 0.0f));
	}

	virtual void OnDestroy() override
	{

	}

private:
	float m_Speed;

	TransformComponent& transform;
};