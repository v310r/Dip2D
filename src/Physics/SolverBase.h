#pragma once

#include <vector>
#include "../Collision/Manifold.h"

class SolverBase
{
public:
	virtual void Solve(std::vector<Manifold> collisions, float deltaTime) = 0;
};

