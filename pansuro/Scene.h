#pragma once

#include <entt/entt.hpp>

#include "Core.h"
#include "MyUUID.h"

class Entity;

class Scene
{
public:
	Scene();
	~Scene() { Shutdown(); }

	void Init();
	void Update(float dt);
	void Render();
	void Shutdown();

	Entity* CreateEntity();
	void DestroyEntityWithID(MyUUID id);

	void LoadAssets();

private:
	void CreateFloor();
	void CreateWood();

private:
	entt::registry m_Registry;

	std::unordered_map<MyUUID, Entity*> m_EntityMap;

	Entity* m_MainCamera;
	Entity* m_Knight;

	friend class Entity;
};