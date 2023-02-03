#pragma once

#include "glm/glm.hpp"

#define NO_COLLISION glm::vec2{-1000.0f, -1000.0f}

class Object;


struct Manifold
{
	Object* One;
	Object* Two;

	float PenetrationDepth;

	glm::vec2 collisionPoint;
};
