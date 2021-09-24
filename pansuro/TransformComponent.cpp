#include "pch.h"
#include "TransformComponent.h"

#include "Engine.h"

TransformComponent::TransformComponent()
	: m_Position(Vector3::Zero)
	, m_Rotation(Vector3::Zero)
	, m_Scale(1.0f)
	, m_UploadBuffer(DEVICE.Get(), 1, true)
{

}

TransformComponent::TransformComponent(const Vector3& position, const Vector3& rotation, float scale)
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
