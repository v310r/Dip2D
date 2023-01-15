#include "CircleCollider.h"
#include "CollisionUtility.h"



CircleCollider::CircleCollider(const glm::vec2 position, const float radius)
	: Collider(position), Radius(radius)
{

}


bool CircleCollider::Collides(const RectangleCollider& other) const
{
	return CollisionUtility::FindCollision(other, *this);
}


bool CircleCollider::Collides(const CircleCollider& other) const
{
	return CollisionUtility::FindCollision(*this, other);
}
