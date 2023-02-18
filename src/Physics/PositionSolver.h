#pragma once

#include "SolverBase.h"

class PositionSolver : public SolverBase
{
public:
	virtual void Solve(std::vector<Collision> collisions, float deltaTime);
};
