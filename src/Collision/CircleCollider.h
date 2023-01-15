#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider(const glm::vec2 position, const float radius);

	virtual bool Collides(const RectangleCollider& other) const override;
	virtual bool Collides(const CircleCollider& other) const override;

private:
	friend class CollisionUtility;

	float Radius;
};

