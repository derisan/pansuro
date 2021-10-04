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
#include "BoxComponent.h"

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
		auto& boxBox1 = m_Box->GetComponent<BoxComponent>();
		auto& knightBox = m_Knight->GetComponent<BoxComponent>();

		if(Intersect(boxBox1, knightBox))
		{
			float dx1 = boxBox1.GetWorldAABB().Max.x - knightBox.GetWorldAABB().Min.x;
			float dx2 = boxBox1.GetWorldAABB().Min.x - knightBox.GetWorldAABB().Max.x;
			float dy1 = boxBox1.GetWorldAABB().Max.y - knightBox.GetWorldAABB().Min.y;
			float dy2 = boxBox1.GetWorldAABB().Min.y - knightBox.GetWorldAABB().Max.y;
			float dz1 = boxBox1.GetWorldAABB().Max.z - knightBox.GetWorldAABB().Min.z;
			float dz2 = boxBox1.GetWorldAABB().Min.z - knightBox.GetWorldAABB().Max.z;

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
		m_Knight = CreateEntity(L"knight");
		m_Knight->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Knight.gpmesh"), ResourceManager::GetTexture(L"Assets/Knight.png"));
		auto& animComponent = m_Knight->AddComponent<AnimatorComponent>(ResourceManager::GetSkeleton(L"Assets/Knight.gpskel"));
		animComponent.PlayAnimation(ResourceManager::GetAnimation(L"Assets/Idle.gpanim"));
		m_Knight->AddComponent<ScriptComponent>(new CharacterMovement(m_Knight, 150.0f));
		m_Knight->AddComponent<DebugDrawComponent>(ResourceManager::GetDebugMesh(L"Assets/Knight.gpmesh", true));
		m_Knight->AddComponent<BoxComponent>(ResourceManager::GetMesh(L"Assets/Knight.gpmesh")->GetAABB());
	}

	{
		m_Box = CreateEntity(L"box");
		m_Box->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Crate.gpmesh"), ResourceManager::GetTexture(L"Assets/Crate.png"));
		auto& tr = m_Box->GetComponent<TransformComponent>();
		tr.SetPosition(Vector3(250.0f, 50.0f, 0.0f));
		m_Box->AddComponent<DebugDrawComponent>(ResourceManager::GetDebugMesh(L"Assets/Crate.gpmesh"));
		m_Box->AddComponent<BoxComponent>(ResourceManager::GetMesh(L"Assets/Crate.gpmesh")->GetAABB());
	}

	//{
	//	m_Box2 = CreateEntity(L"box");
	//	m_Box2->AddComponent<MeshRendererComponent>(ResourceManager::GetMesh(L"Assets/Crate.gpmesh"), ResourceManager::GetTexture(L"Assets/Crate.png"));
	//	auto& tr = m_Box2->GetComponent<TransformComponent>();
	//	tr.SetPosition(Vector3(0.0f, 50.0f, 0.0f));
	//	m_Box2->AddComponent<DebugDrawComponent>(ResourceManager::GetDebugMesh(L"Assets/Crate.gpmesh"));
	//	m_Box2->AddComponent<BoxComponent>(ResourceManager::GetMesh(L"Assets/Crate.gpmesh")->GetAABB());
	//	m_Box2->AddComponent<ScriptComponent>(new CharacterMovement(m_Box2, 150.0f));
	//}

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