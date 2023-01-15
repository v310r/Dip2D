#pragma once

#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"

class RectangleCollider;
class CircleCollider;

class Collider
{
public:

	Collider(const glm::vec2 position) : Position(position) {}

	virtual ~Collider() {}

	virtual bool Collides(const Collider& otherCollider) const
	{
		return otherCollider.Collides(*this);
	}

	virtual bool Collides(const RectangleCollider& other) const = 0;

	virtual bool Collides(const CircleCollider& other) const = 0;
	
	struct
	{
		 glm::vec2 Position;
	};
};

