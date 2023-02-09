#pragma once

#include "glm/glm.hpp"


class Object;

struct CollisionManifold
{
	float PenetrationDepth = std::numeric_limits<float>::max();

	glm::vec2 normal = glm::vec2(0.0f, 1.0f);

	std::vector<glm::vec2> contacts;

	bool IsColliding = false;
};

struct Collision
{
	Object* A;
	Object* B;

	CollisionManifold Manifold;
};