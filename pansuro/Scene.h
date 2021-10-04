#pragma once

#include <entt/entt.hpp>

#include "Core.h"
#include  "MyUUID.h"

class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	void OnInit();
	void OnUpdate(float dt);
	void OnRender();
	void OnDestroy();

	void OnKeyDown(UINT8 keycode);
	void OnKeyUp(UINT8 keycode);

	Entity* CreateEntity(const std::wstring& tag = L"default");
	void DestroyEntityWithID(MyUUID id);

	void LoadAssets();

private:
	entt::registry m_Registry;

	std::unordered_map<MyUUID, Entity*> m_EntityMap;

	Entity* m_MainCamera;
	Entity* m_Box;
	Entity* m_Knight;

	friend class Entity;
};