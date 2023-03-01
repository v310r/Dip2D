#pragma once

#include "SolverBase.h"

class PositionSolver : public SolverBase
{
public:
	virtual void Solve(const std::vector<Collision>& collisions, float deltaTime);
};
