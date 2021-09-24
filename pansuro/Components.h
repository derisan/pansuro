#pragma once

#include "pch.h"
#include "Mesh.h"
#include "Texture.h"
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
