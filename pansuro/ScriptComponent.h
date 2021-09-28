#pragma once

#include "Core.h"

class Script;

class ScriptComponent
{
public:
	ScriptComponent(Script* script);
	~ScriptComponent();

	void OnCreate();
	void OnUpdate(float dt);

	bool bCreated = false;
	
private:
	Script* m_Script;
};

