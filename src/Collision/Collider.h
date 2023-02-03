#pragma once

#include "SFML/Graphics.hpp"
#include "Manifold.h"

class RectangleCollider;
class CircleCollider;

class Collider
{
public:

	Collider(const glm::vec2 position) : Position(position) {}

	virtual ~Collider() {}

	virtual glm::vec2 Collides(const Collider& otherCollider) const
	{
		return otherCollider.Collides(*this);
	}

	virtual glm::vec2 Collides(const RectangleCollider& other) const = 0;

	virtual glm::vec2 Collides(const CircleCollider& other) const = 0;
	
	struct
	{
		 glm::vec2 Position;
	};
};

