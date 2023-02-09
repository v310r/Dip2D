#pragma once

#include "CollisionManifold.h"

class RectangleCollider;
class CircleCollider;

class CollisionUtility
{
public:
	static CollisionManifold GetCollisionManifold(const RectangleCollider& rectangle, const CircleCollider& sphere);

	// deprecated
	static CollisionManifold GetCollisionManifold(const RectangleCollider& rectangle1, const RectangleCollider& rectangle2);

	static CollisionManifold GetCollisionManifold(const CircleCollider& circle1, const CircleCollider& circle2);
};