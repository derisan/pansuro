#pragma once

#include "Entity.h"

class Script
{
public:
	Script(Entity* owner)
		: m_Owner(owner) {}
	virtual ~Script() = default;

	template<typename T>
	T& GetComponent()
	{
		return m_Owner->GetComponent<T>();
	}

	virtual void OnCreate() {}
	virtual void OnUpdate(float dt) {}
	virtual void OnDestroy() {}

protected:
	void Destroy()
	{
		m_Owner->Destroy();
	}

private:
	Entity* m_Owner;
};

