#pragma once

#include "Scene.h"
#include "MyUUID.h"
#include "IDComponent.h"
#include "TransformComponent.h"

class Entity
{
public:
	Entity(entt::entity handle, Scene* scene);

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		MK_ASSERT(!HasComponent<T>(), "Entity already has component!");
		T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename T>
	T& GetComponent()
	{
		MK_ASSERT(HasComponent<T>(), "Entity does not have component!");
		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		MK_ASSERT(HasComponent<T>(), "Entity does not have component!");
		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	operator bool() const { return m_EntityHandle != entt::null; }
	operator entt::entity() const { return m_EntityHandle; }

	bool operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	MyUUID GetUUID() { return GetComponent<IDComponent>().ID; }
	TransformComponent& Tranfrom() { return GetComponent<TransformComponent>(); }

	void Destroy()
	{
		m_Scene->DestroyEntityWithID(GetComponent<IDComponent>().ID);
	}

private:
	entt::entity m_EntityHandle = entt::null;
	Scene* m_Scene = nullptr;
};

