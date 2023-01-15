#pragma once

#include "Collider.h"

class RectangleCollider : public Collider
{
public:
	RectangleCollider(const glm::vec2 position, const glm::vec2 min, const glm::vec2 max);

	virtual bool Collides(const RectangleCollider& other) const override;
	virtual bool Collides(const CircleCollider& other) const override;

private:
	friend class CollisionUtility;

	glm::vec2 Min;
	glm::vec2 Max;
};

