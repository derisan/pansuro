#include "pch.h"
#include "Scene.h"

#include "Engine.h"
#include "ResourceManager.h"
#include "TextureDescriptorHeap.h"
#include "Entity.h"
#include "CharacterMovement.h"
#include "Components.h"


Scene::Scene()
	: m_MainCamera(nullptr)
{

}

void Scene::Init()
{

}

void Scene::Update(float dt)
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

	{
		auto view = m_Registry.view<BoxComponent, TransformComponent>();
		for (auto entity : view)
		{
			// TODO: if transform is dirty, do update boxcomponent.

			auto [box, transform] = view.get<BoxComponent, TransformComponent>(entity);
			box.Update(transform);
		}
	}

	{
		auto& knightBox = m_Knight->GetComponent<BoxComponent>();

		auto view = m_Registry.view<Resource, BoxComponent>();
		for (auto entity : view)
		{
			auto& resourceBox = view.get<BoxComponent>(entity);

			if (Intersect(resourceBox, knightBox))
			{
				float dx1 = resourceBox.GetWorldAABB().Max.x - knightBox.GetWorldAABB().Min.x;
				float dx2 = resourceBox.GetWorldAABB().Min.x - knightBox.GetWorldAABB().Max.x;
				float dy1 = resourceBox.GetWorldAABB().Max.y - knightBox.GetWorldAABB().Min.y;
				float dy2 = resourceBox.GetWorldAABB().Min.y - knightBox.GetWorldAABB().Max.y;
				float dz1 = resourceBox.GetWorldAABB().Max.z - knightBox.GetWorldAABB().Min.z;
				float dz2 = resourceBox.GetWorldAABB().Min.z - knightBox.GetWorldAABB().Max.z;

				float dx = abs(dx1) < abs(dx2) ?
					dx1 : dx2;
				float dy = abs(dy1) < abs(dy2) ?
					dy1 : dy2;
				float dz = abs(dz1) < abs(dz2) ?
					dz1 : dz2;

				TransformComponent& transform = m_Knight->GetComponent<TransformComponent>();
				Vector3 pos = transform.GetPosition();

				if (abs(dx) <= abs(dy) && abs(dx) <= abs(dz))
				{
					pos.x += dx;
				}
				else if (abs(dy) <= abs(dx) && abs(dy) <= abs(dz))
				{
					pos.y += dy;
				}
				else
				{
					pos.z += dz;
				}

				transform.SetPosition(pos);
				knightBox.Update(transform);
			}
		}
	}
}

void Scene::Render()
{
	auto& cameraComponent = m_MainCamera->GetComponent<CameraComponent>();
	cameraComponent.Bind();

	ID3D12DescriptorHeap* ppHeaps[] = { TEXHEAP->GetHeap().Get() };
	CMD_LIST->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	{
		CMD_LIST->SetPipelineState(ENGINE->GetSkinnedPSO().Get());
		auto view = m_Registry.view<AnimatorComponent, MeshRendererComponent, TransformComponent>();
		for (auto entity : view)
		{
			auto [animator, meshRenderer, transform] = view.get<AnimatorComponent, MeshRendererComponent, TransformComponent>(entity);
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

void Scene::Shutdown()
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
	m_MainCamera = CreateEntity();
	auto& camera = m_MainCamera->AddComponent<CameraComponent>(Vector3(0.0f, 700.0f, 0.0f));
	m_MainCamera->AddComponent<Camera>();

	{
		m_Knight = CreateEntity();
		m_Knight->AddComponent<Player>();
		m_Knight->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Knight.gpmesh"), ResourceManager::GetTexture(L"Assets/Knight.png"));
		m_Knight->AddComponent<AnimatorComponent>(ResourceManager::GetSkeleton(L"Assets/Knight.gpskel"));
		m_Knight->AddComponent<ScriptComponent>(new CharacterMovement(m_Knight, 200.0f));
		m_Knight->AddComponent<DebugDrawComponent>(ResourceManager::GetDebugMesh(L"Assets/Knight.gpmesh", true));
		m_Knight->AddComponent<BoxComponent>(ResourceManager::GetMesh(L"Assets/Knight.gpmesh")->GetAABB());
	}

	camera.SetFollowEntity(m_Knight);

	CreateFloor();
	CreateWood();
}

void Scene::CreateFloor()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			auto box = CreateEntity();
			box->AddComponent<Floor>();
			box->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Cube.gpmesh"), ResourceManager::GetTexture(L"Assets/Green.png"));
			auto& tr = box->GetComponent<TransformComponent>();
			tr.SetPosition(Vector3(j * 100.0f, -100.0f, i * 100.0f));
		}
	}
}

void Scene::CreateWood()
{
	auto box = CreateEntity();
	box->AddComponent<Resource>();
	box->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Cube.gpmesh"), ResourceManager::GetTexture(L"Assets/Brown.png"));
	auto& tr = box->GetComponent<TransformComponent>();
	tr.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	box->AddComponent<DebugDrawComponent>(ResourceManager::GetDebugMesh(L"Assets/Cube.gpmesh"));
	box->AddComponent<BoxComponent>(ResourceManager::GetMesh(L"Assets/Cube.gpmesh")->GetAABB());
}

Entity* Scene::CreateEntity()
{
	Entity* entity = new Entity(m_Registry.create(), this);
	entity->AddComponent<TransformComponent>();
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