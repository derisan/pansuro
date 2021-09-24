#include "pch.h"
#include "CameraComponent.h"

#include "Engine.h"

CameraComponent::CameraComponent(const Vector3& position, const Vector3& target, const Vector3& up, float fov)
	: m_Position(position)
	, m_Target(target)
	, m_Up(up)
	, m_UploadBuffer(DEVICE.Get(), 1, true)
{
	m_FOV = XMConvertToRadians(fov);
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Bind()
{
	Matrix viewproj = XMMatrixLookAtLH(m_Position, m_Target, m_Up);
	viewproj *= XMMatrixPerspectiveFovLH(m_FOV, ENGINE->GetAspectRatio(), 0.1f, 1000.0f);
	m_UploadBuffer.CopyData(0, viewproj);
	CMD_LIST->SetGraphicsRootConstantBufferView(RP_ViewProj, m_UploadBuffer.GetVirtualAddress());
}
