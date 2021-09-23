#pragma once

#include "Mesh.h"
#include "d3dHelper.h"
#include "Engine.h"

struct MeshRendererComponent
{
	Mesh* Messi;
	Texture* Tex;

	MeshRendererComponent() = default;
	MeshRendererComponent(const MeshRendererComponent&) = default;
	MeshRendererComponent(Mesh* mesh, Texture* tex)
		: Messi(mesh), Tex(tex) {}
};

struct TransformComponent
{
	Vector3 Position;
	UploadBuffer<Vector3> CBuffer;

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const Vector3& position)
		: Position(position)
	, CBuffer(DEVICE.Get(), 1, true) {}

	operator const D3D12_GPU_VIRTUAL_ADDRESS () const { return CBuffer.GetVirtualAddress(); }
};

struct CameraComponent
{
	Vector3 Position;
	Vector3 Target;
	Vector3 Up;
	float FOV;
	UploadBuffer<Matrix> CBuffer;

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
	CameraComponent(const Vector3& pos, const Vector3& target, const Vector3& up, float fov = 60.0f)
		: Position(pos), Target(target), Up(up), FOV(fov), CBuffer(DEVICE.Get(), 1, true) {}

	operator const Matrix const () 
	{ 
		Matrix viewproj = XMMatrixLookAtLH(Position, Target, Up);
		viewproj *= XMMatrixPerspectiveFovLH(XMConvertToRadians(FOV), ENGINE->GetAspectRatio(), 0.1f, 1000.0f);
		return viewproj;
	}
};