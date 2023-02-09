#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider(const glm::vec2 position, const float radius);

	virtual CollisionManifold Collides(const Collider& other) const override;
	virtual CollisionManifold Collides(const RectangleCollider& other) const override;
	virtual CollisionManifold Collides(const CircleCollider& other) const override;

private:
	friend class CollisionUtility;

	float Radius;
};

