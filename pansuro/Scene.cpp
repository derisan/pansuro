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
#include "TagComponent.h"
#include "Entity.h"

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

	auto view = m_Registry.view<TagComponent, TransformComponent>();
	for (auto entity : view)
	{
		auto [tag, transform] = view.get<TagComponent, TransformComponent>(entity);

		if (tag == L"Box")
		{
			transform.SetRotation(Vector3(0.0f, rot, 0.0f));
		}
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
	camera = m_Registry.create();
	m_Registry.emplace<CameraComponent>(camera, Vector3(0.0f, 2.0f, -2.0f), Vector3::Backward);

	m_Box = CreateEntity(L"Box");
	m_Box->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Cube.gpmesh"), ResourceManager::GetTexture(L"Assets/Cube.png"));
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

	if (keycode == VK_RETURN)
	{
		m_Registry.destroy(*m_Box);
		delete m_Box;
	}
}

void Scene::OnKeyUp(UINT8 keycode)
{

}

Entity* Scene::CreateEntity(const std::wstring& tag)
{
	Entity* entity = new Entity(m_Registry.create(), this);
	entity->AddComponent<TransformComponent>();
	entity->AddComponent<TagComponent>(tag);
	return entity;
}

void Scene::DestroyEntity(Entity entity)
{
	m_Registry.destroy(entity);
}
