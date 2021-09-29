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
		auto rot = transform.GetRotation();

		rot.y += m_Speed * dt;
		transform.SetRotation(rot);
	}

	virtual void OnDestroy() override
	{

	}

private:
	float m_Speed;

	TransformComponent& transform;
};