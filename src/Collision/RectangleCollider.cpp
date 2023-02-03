#include "RectangleCollider.h"
#include "CollisionUtility.h"


RectangleCollider::RectangleCollider(const glm::vec2 position, const glm::vec2 min, const glm::vec2 max)
	: Collider(position), Min(min), Max(max)
{

}

glm::vec2 RectangleCollider::Collides(const RectangleCollider& other) const
{
	return CollisionUtility::FindCollision(*this, other);
}

glm::vec2 RectangleCollider::Collides(const CircleCollider& other) const
{
	return CollisionUtility::FindCollision(*this, other);
}
