#pragma once

#include <vector>
#include "../Objects/Object.h"
#include "SolverBase.h"

class PhysicsWorld
{

public:

	PhysicsWorld(const std::vector<Object*>& objects) : m_objectsRef(objects)
	{
	}

	void AddObject(Object* const object);
	void RemoveObject(Object* const object);

	void AddSolverBase(SolverBase* const object);
	void RemoveSolverBase(SolverBase* const object);

	void Step(const float deltaTime);

	void ResolveCollisions(const float deltaTime);

private:
	const std::vector<Object*>& m_objectsRef;
	
	std::vector<SolverBase*> m_SolverBases;

	const glm::vec2 m_gravity = { 0.0f, 9.81f };
};

