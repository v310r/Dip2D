#pragma once

#include "CollisionManifold.h"

class RectangleCollider;
class CircleCollider;

inline bool CMP(const float x, const float y)
{
	return fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y)));
}

class CollisionUtility
{
public:
	static CollisionManifold GetCollisionManifold(const RectangleCollider& rectangle, const CircleCollider& sphere);

	// deprecated
	static CollisionManifold GetCollisionManifold(const RectangleCollider& rectangle1, const RectangleCollider& rectangle2);

	static CollisionManifold GetCollisionManifold(const CircleCollider& circle1, const CircleCollider& circle2);
};