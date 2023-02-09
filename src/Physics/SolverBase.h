#pragma once

#include <vector>
#include "../Collision/CollisionManifold.h"

class SolverBase
{
public:
	virtual void Solve(std::vector<Collision> collisions, float deltaTime) = 0;
};

