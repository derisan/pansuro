#pragma once

#include "Core.h"

#include <entt/entt.hpp>

class Mesh;
class Texture;

class Scene
{
public:
	Scene();
	~Scene();

	void OnInit();
	void OnUpdate();
	void OnRender();
	void OnDestroy();

	void OnKeyDown(UINT8 keycode);
	void OnKeyUp(UINT8 keycode);

private:
	void LoadAssets();
	Mesh* CreateTestMesh();

private:
	entt::registry m_Registry;

	// For test
	entt::entity camera;
};