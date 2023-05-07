#include "PhysicsWorld.h"
#include "../Objects/Circle.h"
#include <iterator>
#include "iostream"
#include "../Utility/BS_thread_pool.hpp"
#include <functional>


static BS::thread_pool threadPool(3);

//static BS::thread_pool threadPool(8);

//static BS::thread_pool threadPool(std::thread::hardware_concurrency());

static std::mutex collisionMutex;


PhysicsWorld::~PhysicsWorld()
{
	for (SolverBase* solver : m_solvers)
	{
		delete solver;
	}
}

void PhysicsWorld::AddObject(Object* const object)
{
	//if (object)
	//{
	//	m_objectsRef.push_back(object);
	//}
}

void PhysicsWorld::RemoveObject(Object* const object)
{
	//if (object)
	//{
	//	auto it = std::find(m_objectsRef.begin(), m_objectsRef.end(), object);
	//	if (it != m_objectsRef.end())
	//	{
	//		m_objectsRef.erase(it);
	//	}
	//}
}

void PhysicsWorld::AddSolver(SolverBase* const object)
{
	m_solvers.emplace_back(object);
}

void PhysicsWorld::Step(const float deltaTime)
{
	if (m_objectsRef.size() == 0)
	{
		//m_SimulationTime.restart();
		return;
	}

	ApplyForcesAndUpdatePositionAndVelocity(deltaTime);

	DetectCollisionsForDemo();

	//DetectCollisionsForTests();

	//DetectCollisionsParallelForTests();

	SolveConstraints(m_collisions, deltaTime);

	//SolveConstraintsParallel(deltaTime);

	m_collisions.clear();
}

void PhysicsWorld::ApplyForcesAndUpdatePositionAndVelocity(const float deltaTime)
{
	const float damping = 0.98f; // simulates air friction
	for (Object* const obj : m_objectsRef)
	{
		if (obj->IsStatic())
		{
			continue;
		}

		//std::cout << "obj position y: " << obj->GetPosition().y << " time spent: " << m_SimulationTime.getElapsedTime().asSeconds() << "s\n";
		obj->AddToNetForce(obj->GetMass() * m_gravity); // gravity force component

		const glm::vec2 acceleration = obj->GetNetForce() * obj->GetInvMass();
		obj->AddVelocity(acceleration * deltaTime);

		//obj->ApplyLinearDamping(damping);
		const glm::vec2 offset = obj->GetVelocity() * deltaTime;
		obj->AddToPosition(offset);

		obj->SetNetForce({ 0.0f, 0.0f });
	}
}

void PhysicsWorld::DetectCollisionsForDemo()
{
	//std::vector<Collision> collisions;

	for (Object* const a : m_objectsRef)
	{
		if (!a->GetCollider())
		{
			continue;
		}

		for (Object* const b : m_objectsRef)
		{
			if (a == b)
			{
				break; // same object
			}

			if (!b->GetCollider())
			{
				continue; // no collider present
			}

			if (a->IsStatic() && b->IsStatic())
			{
				continue;
			}

			CollisionManifold manifold = a->GetCollider()->Collides(*b->GetCollider());
			if (manifold.IsColliding)
			{
				m_collisions.emplace_back(Collision{ a, b, manifold });
			}
		}
	}

	//return collisions;
}

void PhysicsWorld::DetectCollisionsForTests()
{
	for (size_t i = 0; i < m_objectsRef.size(); ++i)
	{
		if (!m_objectsRef[i]->GetCollider())
		{
			continue;
		}

		for (size_t j = i; j < m_objectsRef.size(); ++j)
		{
			if (!m_objectsRef[j]->GetCollider())
			{
				continue;
			}

			if (m_objectsRef[i]->IsStatic() && m_objectsRef[j]->IsStatic())
			{
				continue;
			}

			CollisionManifold manifold = m_objectsRef[i]->GetCollider()->Collides(*m_objectsRef[j]->GetCollider());
			if (manifold.IsColliding)
			{
				m_collisions.emplace_back(Collision{ m_objectsRef[i], m_objectsRef[j], manifold });
			}
		}
	}
}

void PhysicsWorld::DetectCollisionsForThread(size_t startIndex, size_t endIndex)
{
	for (size_t i = startIndex; i < endIndex; ++i)
	{
		if (!m_objectsRef[i]->GetCollider())
		{
			continue;
		}

		for (size_t j = i; j < endIndex; ++j)
		{
			if (!m_objectsRef[j]->GetCollider())
			{
				continue;
			}

			if (m_objectsRef[i]->IsStatic() && m_objectsRef[j]->IsStatic())
			{
				continue;
			}

			CollisionManifold manifold = m_objectsRef[i]->GetCollider()->Collides(*m_objectsRef[j]->GetCollider());
			if (manifold.IsColliding)
			{
				std::unique_lock lock(collisionMutex);
				m_collisions.emplace_back(Collision{ m_objectsRef[i], m_objectsRef[j], manifold });
			}
		}
	}
}

void PhysicsWorld::DetectCollisionsParallelForTests()
{
	const size_t numberOfThreads = threadPool.get_thread_count();
	const size_t step = m_objectsRef.size() / numberOfThreads;
	size_t startIndex = 0;
	for (size_t i = 0; i < numberOfThreads; ++i)
	{
		size_t endIndex = ((i == (numberOfThreads - 1)) ? m_objectsRef.size() : startIndex + step);
		threadPool.push_task(&PhysicsWorld::DetectCollisionsForThread, this, startIndex, endIndex);
		startIndex += step;
	}

	threadPool.wait_for_tasks();
}

void PhysicsWorld::SolveConstraints(const std::vector<Collision>& collisions, const float deltaTime)
{
	for (SolverBase* solver : m_solvers)
	{
		solver->Solve(collisions, deltaTime);
	}
}

void PhysicsWorld::SolveConstraintsParallel(float deltaTime)
{
	const int blockSize = std::thread::hardware_concurrency();
	const int elementsPerThread = m_collisions.size() / blockSize;
	if (elementsPerThread == 0)
	{
		SolveConstraints(m_collisions, deltaTime);
		return;
	}

	std::vector<std::vector<Collision>> collisionBuckets;
	for (size_t i = 0; i < m_collisions.size(); i += elementsPerThread) 
	{
		auto first = m_collisions.begin() + i;
		auto last = (i + elementsPerThread < m_collisions.size()) ? first + elementsPerThread : m_collisions.end();

		collisionBuckets.emplace_back(first, last);
	}

	for (int i = 0; i < collisionBuckets.size(); ++i)
	{
		threadPool.push_task(&PhysicsWorld::SolveConstraints, this, collisionBuckets[i], deltaTime);
	}

	threadPool.wait_for_tasks();
}
