#include "RectangleCollider.h"
#include "CollisionUtility.h"


RectangleCollider::RectangleCollider(const glm::vec2 position, const glm::vec2 min, const glm::vec2 max)
	: Collider(position), Min(min), Max(max)
{

}

CollisionManifold RectangleCollider::Collides(const RectangleCollider& other) const
{
	return CollisionUtility::GetCollisionManifold(*this, other);
}

CollisionManifold RectangleCollider::Collides(const CircleCollider& other) const
{
	return CollisionUtility::GetCollisionManifold(*this, other);
}

CollisionManifold RectangleCollider::Collides(const Collider& other) const
{
	return other.Collides(*this);
}
