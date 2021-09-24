#include "pch.h"
#include "Scene.h"

#include "Engine.h"
#include "Texture.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "TextureDescriptorHeap.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::OnInit()
{
	LoadAssets();
}

void Scene::OnUpdate(float dt)
{
	static float rot = 0.0f;

	rot += 90.0f * dt;

	auto view = m_Registry.view<TransformComponent>();
	for (auto entity : view)
	{
		auto& transform = view.get<TransformComponent>(entity);
		transform.SetRotation(Vector3(0.0f, rot, 0.0f));
	}
}

void Scene::OnRender()
{
	auto& cameraComponent = m_Registry.get<CameraComponent>(camera);
	cameraComponent.Bind();

	ID3D12DescriptorHeap* ppHeaps[] = { TEXHEAP->GetHeap().Get() };
	CMD_LIST->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	auto view = m_Registry.view<MeshRendererComponent, TransformComponent>();
	for (auto entity : view)
	{
		auto [meshRenderer, transform] = view.get<MeshRendererComponent, TransformComponent>(entity);
		transform.Bind();
		meshRenderer.Bind();
	}
}

void Scene::OnDestroy()
{
	m_Registry.clear();
}

void Scene::LoadAssets()
{
	auto entt = m_Registry.create();
	m_Registry.emplace<MeshRendererComponent>(entt, ResourceManager::LoadCubeMesh(), ResourceManager::LoadTexture(L"Assets/Textures/veigar.jpg"));
	m_Registry.emplace<TransformComponent>(entt);

	camera = m_Registry.create();
	m_Registry.emplace<CameraComponent>(camera, Vector3(0.0f, 2.0f, -2.0f), Vector3::Backward);
}

void Scene::OnKeyDown(UINT8 keycode)
{
	if (keycode == VK_RIGHT)
	{
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			Vector3 pos = transform.GetPosition();
			pos.x += 0.5f;
			transform.SetPosition(pos);
		}
	}

	if (keycode == VK_LEFT)
	{
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			Vector3 pos = transform.GetPosition();
			pos.x -= 0.5f;
			transform.SetPosition(pos);
		}
	}
}

void Scene::OnKeyUp(UINT8 keycode)
{

}
