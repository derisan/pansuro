#pragma once

#include "Core.h"

class Mesh;

class DebugDrawComponent
{
public:
	DebugDrawComponent();
	DebugDrawComponent(Mesh* mesh);

	void Bind();

	// TODO
	// Entity의 이동 변환만 적용되도록..
	
private:
	Mesh* m_Mesh;
};

