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
