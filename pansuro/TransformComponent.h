#pragma once

#include "Core.h"

class TransformComponent
{
public:
	TransformComponent();
	TransformComponent(const Vector3& position, const Vector3& rotation = Vector3::Zero, const Vector3& scale = Vector3::One);
	~TransformComponent();

	void Bind();

	void SetPosition(const Vector3& position) { m_Position = position; }
	const Vector3& GetPosition() const { return m_Position; }

	void SetRotation(const Vector3& rotation) { m_Rotation = rotation; }
	const Vector3& GetRotation() const { return m_Rotation; }

	void SetScale(const Vector3& scale) { m_Scale = scale; }
	void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) { m_Scale = Vector3(x, y, z); }
	const Vector3& GetScale() const { return m_Scale; }

	void RotateYaw(float yaw, bool bAccumulate = false);
	void MoveForward(float speed);

	Vector3 GetForward() const;

private:
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;

	UploadBuffer<Matrix> m_UploadBuffer;
};

