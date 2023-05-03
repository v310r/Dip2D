#pragma once

#include <vector>
#include "../Objects/Object.h"
#include "SolverBase.h"

class PhysicsWorld
{

public:

	PhysicsWorld(const std::vector<Object*>& objects) : m_objectsRef(objects)
	{
		m_collisions.reserve(10000);
	}

	~PhysicsWorld();

	void AddObject(Object* const object);
	void RemoveObject(Object* const object);

	void AddSolver(SolverBase* const object);
	void RemoveSolver(SolverBase* const object);

	void Step(const float deltaTime);

private:
	void ApplyForcesAndUpdatePositionAndVelocity(const float deltaTime);

	void DetectCollisions();

	void DetectCollisionsForTests();

	void DetectCollisionsParallelForTests(size_t startIndex, size_t endIndex);

	void SolveConstraints(const std::vector<Collision>& collisions, const float deltaTime);


	const std::vector<Object*>& m_objectsRef;
	std::vector<SolverBase*> m_solvers;

	const glm::vec2 m_gravity = { 0.0f, 198.1f };

	std::vector<Collision> m_collisions;

	//sf::Clock m_SimulationTime;
};

