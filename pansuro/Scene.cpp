#include "pch.h"
#include "Scene.h"

#include "Engine.h"
#include "Components.h"
#include "Texture.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "TextureDescriptorHeap.h"
#include "CameraComponent.h"

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
	auto view = m_Registry.view<TransformComponent>();
	for (auto entity : view)
	{
		auto& transform = view.get<TransformComponent>(entity);
		transform.CBuffer.CopyData(0, transform.Position);
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
		CMD_LIST->SetGraphicsRootConstantBufferView(RP_World, transform);
		CMD_LIST->SetGraphicsRootDescriptorTable(RP_Texture, meshRenderer.Tex->GetGpuHandle());
		SUBMIT(meshRenderer.Messi);
	}
}

void Scene::OnDestroy()
{
	m_Registry.clear();
}

void Scene::LoadAssets()
{
	auto entt = m_Registry.create();
	m_Registry.emplace<MeshRendererComponent>(entt, ResourceManager::LoadCubeMesh(), ResourceManager::LoadTexture(L"Assets/Textures/cat.png"));
	m_Registry.emplace<TransformComponent>(entt, Vector3(0.0f, 0.1f, 0.0f));

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
			transform.Position.x += 0.1f;
		}
	}
}

void Scene::OnKeyUp(UINT8 keycode)
{

}
