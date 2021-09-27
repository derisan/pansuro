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
#include "IDComponent.h"
#include "TagComponent.h"
#include "Entity.h"
#include "Animation.h"
#include "Skeleton.h"
#include "AnimatorComponent.h"

Scene::Scene()
	: m_MainCamera(nullptr)
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
	static float rot;

	rot += 30.0f * dt;

	auto view = m_Registry.view<AnimatorComponent, TransformComponent>();
	for (auto entity : view)
	{
		auto [animator, transform] = view.get<AnimatorComponent, TransformComponent>(entity);
		animator.Update(dt);
		transform.SetRotation(Vector3(0.0f, rot, 0.0f));
	}
}

void Scene::OnRender()
{
	auto& cameraComponent = m_MainCamera->GetComponent<CameraComponent>();
	cameraComponent.Bind();

	ID3D12DescriptorHeap* ppHeaps[] = { TEXHEAP->GetHeap().Get() };
	CMD_LIST->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	auto group = m_Registry.group<MeshRendererComponent, AnimatorComponent>(entt::get<TransformComponent>);
	for (auto entity : group)
	{
		auto [meshRenderer, transform] = group.get<MeshRendererComponent, TransformComponent>(entity);
		auto& animator = group.get<AnimatorComponent>(entity);
		animator.Bind();
		transform.Bind();
		meshRenderer.Bind();
	}
}

void Scene::OnDestroy()
{
	m_Registry.clear();

	for (auto& [_, entity] : m_EntityMap)
	{
		delete entity;
	}
	m_EntityMap.clear();
}

void Scene::LoadAssets()
{
	m_MainCamera = CreateEntity(L"MainCamera");
	m_MainCamera->AddComponent<CameraComponent>(Vector3(0.0f, 100.0f, -300.0f), Vector3::Backward);

	auto box = CreateEntity(L"Box");
	box->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Knight.gpmesh"), ResourceManager::GetTexture(L"Assets/Knight.png"));
	auto& animComponent = box->AddComponent<AnimatorComponent>(ResourceManager::GetSkeleton(L"Assets/Knight.gpskel"));
	animComponent.PlayAnimation(ResourceManager::GetAnimation(L"Assets/Run.gpanim"));
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
		auto group = m_Registry.group<TagComponent>(entt::get<IDComponent>);
		for (auto entity : group)
		{
			auto [tag, id] = group.get<TagComponent, IDComponent>(entity);

			if (tag == L"Box")
			{
				DestroyEntityWithID(id.ID);
			}
		}
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
	auto& id = entity->AddComponent<IDComponent>();

	m_EntityMap[id.ID] = entity;

	return entity;
}

void Scene::DestroyEntityWithID(MyUUID id)
{
	auto iter = m_EntityMap.find(id);

	if (iter != m_EntityMap.end())
	{
		m_Registry.destroy(*(iter->second));
		delete iter->second;
		m_EntityMap.erase(iter);
	}
}