#pragma once

#include "SFML/Graphics.hpp"
#include "CollisionManifold.h"

class RectangleCollider;
class CircleCollider;

class Collider
{
public:

	Collider(const glm::vec2 position) : Position(position) {}

	virtual ~Collider() {}

	virtual CollisionManifold Collides(const Collider& other) const = 0;

	virtual CollisionManifold Collides(const RectangleCollider& other) const = 0;

	virtual CollisionManifold Collides(const CircleCollider& other) const = 0;
	
	struct
	{
		 glm::vec2 Position;
	};
};

