#pragma once

#include "Manifold.h"

class RectangleCollider;
class CircleCollider;

class CollisionUtility
{
public:
	static glm::vec2 FindCollision(const RectangleCollider& rectangle, const CircleCollider& sphere);

	// deprecated
	static glm::vec2 FindCollision(const RectangleCollider& rectangle1, const RectangleCollider& rectangle2);

	static glm::vec2 FindCollision(const CircleCollider& sphere1, const CircleCollider& sphere2);
};