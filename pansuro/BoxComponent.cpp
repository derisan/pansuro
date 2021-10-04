#include "pch.h"
#include "BoxComponent.h"

BoxComponent::BoxComponent()
	: m_Box{}
	, m_WorldBox{}
{

}

BoxComponent::BoxComponent(const AABB& aabb)
	: m_Box(aabb)
	, m_WorldBox{}
{
	
}

void BoxComponent::Update(const TransformComponent& transform)
{
	m_WorldBox = m_Box;

	m_WorldBox.Rotate(transform.GetRotation());
	m_WorldBox.Min += transform.GetPosition();
	m_WorldBox.Max += transform.GetPosition();
}
