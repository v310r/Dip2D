#include "CircleCollider.h"
#include "CollisionUtility.h"



CircleCollider::CircleCollider(const glm::vec2 position, const float radius)
	: Collider(position), Radius(radius)
{

}


CollisionManifold CircleCollider::Collides(const RectangleCollider& other) const
{
	return CollisionUtility::GetCollisionManifold(other, *this);
}

 
CollisionManifold CircleCollider::Collides(const CircleCollider& other) const
{
	return CollisionUtility::GetCollisionManifold(*this, other);
}

CollisionManifold CircleCollider::Collides(const Collider& other) const
{
	return other.Collides(*this);
}
