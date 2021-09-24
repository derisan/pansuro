#pragma once

#include "Core.h"

class Engine;

class CameraComponent
{
public:
	CameraComponent(const Vector3& position, const Vector3& target, const Vector3& up = Vector3::UnitY, float fov = 90.0f);
	~CameraComponent();

	void Bind();

private:
	Vector3 m_Position;
	Vector3 m_Target;
	Vector3 m_Up;

	float m_FOV;

	UploadBuffer<Matrix> m_UploadBuffer;
};

