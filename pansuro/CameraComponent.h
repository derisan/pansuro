#pragma once

#include "Core.h"

class Engine;
class Entity;

class CameraComponent
{
public:
	CameraComponent(const Vector3& position, const Vector3& target = Vector3::Backward, const Vector3& up = Vector3::UnitY, float fov = 90.0f);

	void Bind();
	void SetFollowEntity(Entity* entity) { m_FollowEntity = entity; }

private:
	Vector3 m_Position;
	Vector3 m_Target;
	Vector3 m_Up;

	float m_FOV;

	UploadBuffer<Matrix> m_UploadBuffer;

	Entity* m_FollowEntity;
};

