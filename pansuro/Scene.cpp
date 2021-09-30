#include "pch.h"
#include "Scene.h"

#include "Engine.h"
#include "ResourceManager.h"
#include "TextureDescriptorHeap.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "IDComponent.h"
#include "TagComponent.h"
#include "Entity.h"
#include "AnimatorComponent.h"
#include "CharacterMovement.h"
#include "ScriptComponent.h"
#include "DebugDrawComponent.h"

Scene::Scene()
	: m_MainCamera(nullptr)
{

}

Scene::~Scene()
{

}

void Scene::OnInit()
{

}

void Scene::OnUpdate(float dt)
{
	{
		auto view = m_Registry.view<ScriptComponent>();
		for (auto entity : view)
		{
			auto& sc = view.get<ScriptComponent>(entity);

			if (sc.bCreated == false)
			{
				sc.OnCreate();
			}

			sc.OnUpdate(dt);
		}
	}

	{
		auto view = m_Registry.view<AnimatorComponent>();
		for (auto entity : view)
		{
			auto& animator = view.get<AnimatorComponent>(entity);
			animator.Update(dt);
		}
	}
}

void Scene::OnRender()
{
	auto& cameraComponent = m_MainCamera->GetComponent<CameraComponent>();
	cameraComponent.Bind();

	ID3D12DescriptorHeap* ppHeaps[] = { TEXHEAP->GetHeap().Get() };
	CMD_LIST->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	{
		CMD_LIST->SetPipelineState(ENGINE->GetSkinnedPSO().Get());
		auto group = m_Registry.view<MeshRendererComponent, AnimatorComponent, TransformComponent>();
		for (auto entity : group)
		{
			auto [meshRenderer, transform] = group.get<MeshRendererComponent, TransformComponent>(entity);
			auto& animator = group.get<AnimatorComponent>(entity);
			animator.Bind();
			transform.Bind();
			meshRenderer.Bind();
		}
	}

	{
		CMD_LIST->SetPipelineState(ENGINE->GetDefaultPSO().Get());
		auto view = m_Registry.view<MeshRendererComponent, TransformComponent>(entt::exclude<AnimatorComponent>);
		for (auto entity : view)
		{
			auto [mr, transform] = view.get<MeshRendererComponent, TransformComponent>(entity);
			transform.Bind();
			mr.Bind();
		}
	}

	{
		CMD_LIST->SetPipelineState(ENGINE->GetDebugPSO().Get());
		auto view = m_Registry.view<DebugDrawComponent, TransformComponent>();
		for (auto entity : view)
		{
			auto [dd, tr] = view.get<DebugDrawComponent, TransformComponent>(entity);
			tr.Bind();
			dd.Bind();
		}
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
	m_MainCamera->AddComponent<CameraComponent>(Vector3(0.0f, 200.0f, -300.0f), Vector3::Backward);

	{
		auto knight = CreateEntity(L"knight");
		knight->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Knight.gpmesh"), ResourceManager::GetTexture(L"Assets/Knight.png"));
		auto& animComponent = knight->AddComponent<AnimatorComponent>(ResourceManager::GetSkeleton(L"Assets/Knight.gpskel"));
		animComponent.PlayAnimation(ResourceManager::GetAnimation(L"Assets/Idle.gpanim"));
		knight->AddComponent<ScriptComponent>(new CharacterMovement(knight, 150.0f));
		knight->AddComponent<DebugDrawComponent>(ResourceManager::GetDebugMesh(L"Assets/Knight.gpmesh"));
	}

	{
		m_Box = CreateEntity(L"box");
		m_Box->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Crate.gpmesh"), ResourceManager::GetTexture(L"Assets/Crate.png"));
		auto& tr = m_Box->GetComponent<TransformComponent>();
		tr.SetPosition(Vector3(250.0f, 50.0f, 0.0f));
		m_Box->AddComponent<DebugDrawComponent>(ResourceManager::GetDebugMesh(L"Assets/Crate.gpmesh"));
	}

	//{
	//	auto plane = CreateEntity(L"plane");
	//	plane->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Plane.gpmesh"), ResourceManager::GetTexture(L"Assets/Plane.png"));
	//	auto& tr = plane->GetComponent<TransformComponent>();
	//}
}

void Scene::OnKeyDown(UINT8 keycode)
{

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