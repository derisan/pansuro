#pragma once

#include "Core.h"

class TransformComponent
{
public:
	TransformComponent();
	TransformComponent(const Vector3& position, const Vector3& rotation = Vector3::Zero, float scale = 1.0f);
	~TransformComponent();

	void Bind();

	void SetPosition(const Vector3& position) { m_Position = position; }
	const Vector3& GetPosition() const { return m_Position; }

	void SetRotation(const Vector3& rotation) { m_Rotation = rotation; }
	const Vector3& GetRotation() const { return m_Rotation; }

	void SetScale(float scale) { m_Scale = scale; }
	float GetScale() const { return m_Scale; }

private:
	Vector3 m_Position;
	Vector3 m_Rotation;
	float m_Scale;

	UploadBuffer<Matrix> m_UploadBuffer;
};

