#include "pch.h"
#include "Scene.h"

#include "Engine.h"
#include "Components.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TextureDescriptorHeap.h"

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

void Scene::OnUpdate()
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
	cameraComponent.CBuffer.CopyData(0, cameraComponent);

	CMD_LIST->SetGraphicsRootConstantBufferView(2, cameraComponent.CBuffer.GetVirtualAddress());

	ID3D12DescriptorHeap* ppHeaps[] = { TEXHEAP->GetHeap().Get() };
	CMD_LIST->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	auto view = m_Registry.view<MeshRendererComponent, TransformComponent>();
	for (auto entity : view)
	{
		auto [meshRenderer, transform] = view.get<MeshRendererComponent, TransformComponent>(entity);
		CMD_LIST->SetGraphicsRootConstantBufferView(0, transform);
		CMD_LIST->SetGraphicsRootDescriptorTable(1, meshRenderer.Tex->GetGpuHandle());
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
	m_Registry.emplace<CameraComponent>(camera, Vector3(0.0f, 2.0f, -2.0f), Vector3::Backward, Vector3::UnitY, 60);
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

Mesh* Scene::CreateTestMesh()
{
	float aspectRatio = ENGINE->GetAspectRatio();

	std::vector<Vertex> vertices(4);
	vertices[0].Position = Vector3(-0.2f, 0.2f, 0.0f);
	vertices[0].UV = Vector2(0.0f, 0.0f);
	vertices[1].Position = Vector3(0.2f, 0.2f, 0.0f);
	vertices[1].UV = Vector2(1.0f, 0.0f);
	vertices[2].Position = Vector3(0.2f, -0.2f, 0.0f);
	vertices[2].UV = Vector2(1.0f, 1.0f);
	vertices[3].Position = Vector3(-0.2f, -0.2f, 0.0f);
	vertices[3].UV = Vector2(0.0f, 1.0f);

	std::vector<UINT> indices;
	{
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
	}
	{
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
	}

	Mesh* rect = new Mesh(vertices, indices);
	return rect;
}
