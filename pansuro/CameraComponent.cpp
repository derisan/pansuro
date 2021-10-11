#include "pch.h"
#include "CameraComponent.h"

#include "Engine.h"
#include "Entity.h"

CameraComponent::CameraComponent(const Vector3& position, const Vector3& target, const Vector3& up, float fov)
	: m_Position(position)
	, m_Target(target)
	, m_Up(up)
	, m_UploadBuffer(DEVICE.Get(), 1, true)
	, m_FollowEntity(nullptr)
{
	m_FOV = XMConvertToRadians(fov);
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Bind()
{
	if (m_FollowEntity)
	{
		auto& transform = m_FollowEntity->GetComponent<TransformComponent>();
		const auto& entityPos = transform.GetPosition();

		m_Position.x = entityPos.x;
		m_Position.z = entityPos.z - 100.0f;
		
		m_Target = entityPos;
	}

	Matrix viewproj = XMMatrixLookAtLH(m_Position, m_Target, m_Up);
	viewproj *= XMMatrixPerspectiveFovLH(m_FOV, ENGINE->GetAspectRatio(), 0.1f, 10000.0f);
	m_UploadBuffer.CopyData(0, viewproj);
	CMD_LIST->SetGraphicsRootConstantBufferView(RP_ViewProj, m_UploadBuffer.GetVirtualAddress());
}
