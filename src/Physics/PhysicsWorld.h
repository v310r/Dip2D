#pragma once

#include <vector>
#include "../Objects/Object.h"

struct CollidedObject
{
	glm::vec2 CollisionPoint {};
	Object* Ptr = nullptr;
};

struct Collision
{
	CollidedObject CollidedObj1;
	CollidedObject CollidedObj2;
};

class PhysicsWorld
{

public:

	PhysicsWorld(const std::vector<Object*>& objects) : m_objectsRef(objects)
	{
	}

	void AddObject(Object* const object);
	void RemoveObject(Object* const object);

	void Step(const float deltaTime);

	void ResolveCollisions(const float deltaTime);

private:
	const std::vector<Object*>& m_objectsRef;
	
	glm::vec2 m_gravity = { 0.0f, 9.81f };
};

