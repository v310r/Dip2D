#pragma once

#include "SolverBase.h"


class ImpulseSolver : public SolverBase
{
public:
	virtual void Solve(std::vector<Collision> collisions, float deltaTime);
};

