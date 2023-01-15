#pragma once

class RectangleCollider;
class CircleCollider;

class CollisionUtility
{
public:
	static bool FindCollision(const RectangleCollider& rectangle, const CircleCollider& sphere);

	static bool FindCollision(const RectangleCollider& rectangle1, const RectangleCollider& rectangle2);

	static bool FindCollision(const CircleCollider& sphere1, const CircleCollider& sphere2);
};