#pragma once

#include "Core.h"

class Mesh;
class Texture;

class MeshRendererComponent
{
public:
	MeshRendererComponent();
	MeshRendererComponent(Mesh* mesh, Texture* texture);
	~MeshRendererComponent();

	void SetMesh(Mesh* mesh) { m_Mesh = mesh; }
	void SetTexture(Texture* texture) { m_Texture = texture; }

	void Bind();
	
private:
	Mesh* m_Mesh;
	Texture* m_Texture;
};

