#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider(const glm::vec2 position, const float radius);

	virtual glm::vec2 Collides(const RectangleCollider& other) const override;
	virtual glm::vec2 Collides(const CircleCollider& other) const override;

private:
	friend class CollisionUtility;

	float Radius;
};

