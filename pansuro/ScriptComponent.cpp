#include "pch.h"
#include "ScriptComponent.h"

#include "Script.h"
#include "Entity.h"

ScriptComponent::ScriptComponent(Script* script)
	: m_Script(script)
{

}

ScriptComponent::ScriptComponent(ScriptComponent&& other) noexcept
{
	if (m_Script)
	{
		delete m_Script;
		m_Script = other.m_Script;
		other.m_Script = nullptr;
	}
}

ScriptComponent& ScriptComponent::operator=(ScriptComponent&& other) noexcept
{
	if (this != &other)
	{
		if (m_Script)
		{
			delete m_Script;
			m_Script = other.m_Script;
			other.m_Script = nullptr;
		}
	}

	return *this;
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
