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
	// Entity�� �̵� ��ȯ�� ����ǵ���..
	
private:
	Mesh* m_Mesh;
};

