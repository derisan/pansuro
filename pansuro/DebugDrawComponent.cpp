#include "pch.h"
#include "DebugDrawComponent.h"

#include "Mesh.h"
#include "Engine.h"

DebugDrawComponent::DebugDrawComponent()
	: m_Mesh(nullptr)
{

}

DebugDrawComponent::DebugDrawComponent(Mesh* mesh)
	: m_Mesh(mesh)
{

}

void DebugDrawComponent::Bind()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, m_Mesh->GetVertexBufferView());
	CMD_LIST->IASetIndexBuffer(m_Mesh->GetIndexBufferView());
	CMD_LIST->DrawIndexedInstanced(m_Mesh->GetIndexCount(), 1, 0, 0, 0);
}
