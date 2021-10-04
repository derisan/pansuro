#pragma once

#include "Core.h"
#include "TransformComponent.h"

class BoxComponent
{
public:
	BoxComponent();
	BoxComponent(const AABB& aabb);

	void Update(const TransformComponent& transform);

	const AABB& GetLocalAABB() const { return m_Box; }
	const AABB& GetWorldAABB() const { return m_WorldBox; }
	
private:
	AABB m_Box;
	AABB m_WorldBox;
};

inline bool Intersect(const BoxComponent& a, const BoxComponent& b)
{
	auto& aBox = a.GetWorldAABB();
	auto& bBox = b.GetWorldAABB();

	bool no = aBox.Max.x < bBox.Min.x ||
		aBox.Max.y < bBox.Min.y ||
		aBox.Max.z < bBox.Min.z ||
		bBox.Max.x < aBox.Min.x ||
		bBox.Max.y < aBox.Min.y ||
		bBox.Max.z < aBox.Min.z;
	
	return !no;
}
