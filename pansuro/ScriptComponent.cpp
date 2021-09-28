#include "pch.h"
#include "ScriptComponent.h"

#include "Script.h"
#include "Entity.h"

ScriptComponent::ScriptComponent(Script* script)
	: m_Script(script)
{

}

ScriptComponent::~ScriptComponent()
{
	if (m_Script)
	{
		m_Script->OnDestroy();
		delete m_Script;
	}
}

void ScriptComponent::OnCreate()
{
	m_Script->OnCreate();
	bCreated = true;
}

void ScriptComponent::OnUpdate(float dt)
{
	m_Script->OnUpdate(dt);
}
