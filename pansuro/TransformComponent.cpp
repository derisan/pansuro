#include "pch.h"
#include "TransformComponent.h"

#include "Engine.h"

TransformComponent::TransformComponent()
	: m_Position(Vector3::Zero)
	, m_Rotation(Vector3::Zero)
	, m_Scale(Vector3::One)
	, m_UploadBuffer(DEVICE.Get(), 1, true)
{

}

TransformComponent::TransformComponent(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	: m_Position(position)
	, m_Rotation(rotation)
	, m_Scale(scale)
	, m_UploadBuffer(DEVICE.Get(), 1, true)
{

}

TransformComponent::~TransformComponent()
{

}

void TransformComponent::Bind()
{
	Matrix worldMat = Matrix::CreateScale(m_Scale);
	worldMat *= Matrix::CreateRotationY(XMConvertToRadians(m_Rotation.y));
	worldMat *= Matrix::CreateTranslation(m_Position);

	m_UploadBuffer.CopyData(0, worldMat);

	CMD_LIST->SetGraphicsRootConstantBufferView(RP_World, m_UploadBuffer.GetVirtualAddress());
}

void TransformComponent::RotateYaw(float yaw, bool bAccumulate /*= false*/)
{
	if (bAccumulate)
	{
		m_Rotation.y += yaw;
	}

	else
	{
		m_Rotation.y = yaw;
	}
}

void TransformComponent::MoveForward(float speed)
{
	Vector3 forward = XMVector3Rotate(Vector3::Backward, Quaternion::CreateFromAxisAngle(Vector3::UnitY, XMConvertToRadians(m_Rotation.y)));
	m_Position += forward * speed;
}

void TransformComponent::Move(const Vector3& velocity)
{
	m_Position += velocity;
}

Vector3 TransformComponent::GetForward() const
{
	Vector3 forward = XMVector3Rotate(Vector3::Backward, Quaternion::CreateFromAxisAngle(Vector3::UnitY, XMConvertToRadians(m_Rotation.y)));
	return forward;
}
