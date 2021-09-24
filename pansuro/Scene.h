#pragma once

#include "Core.h"

#include <entt/entt.hpp>

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
	void DestroyEntity(Entity entity);

private:
	void LoadAssets();

private:
	entt::registry m_Registry;

	// For test
	entt::entity camera;

	Entity* m_Box;

	friend class Entity;
};