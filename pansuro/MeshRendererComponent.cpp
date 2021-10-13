#include "pch.h"
#include "MeshRendererComponent.h"

#include "Mesh.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Engine.h"

MeshRendererComponent::MeshRendererComponent()
	: m_Mesh(nullptr)
	, m_Texture(nullptr)
{

}

MeshRendererComponent::MeshRendererComponent(Mesh* mesh, Texture* texture)
	: m_Mesh(mesh)
	, m_Texture(texture)
{

}

void MeshRendererComponent::Bind()
{
	CMD_LIST->SetGraphicsRootDescriptorTable(RP_Texture, m_Texture->GetGpuHandle());
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, m_Mesh->GetVertexBufferView());
	CMD_LIST->IASetIndexBuffer(m_Mesh->GetIndexBufferView());
	CMD_LIST->DrawIndexedInstanced(m_Mesh->GetIndexCount(), 1, 0, 0, 0);
}
