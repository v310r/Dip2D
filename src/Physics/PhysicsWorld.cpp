#include "PhysicsWorld.h"
#include "iostream"


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

void PhysicsWorld::ResolveCollisions(const float deltaTime)
{
	std::vector<Collision> collisions;
	for (Object* const a : m_objectsRef)
	{
		for (Object* const b : m_objectsRef)
		{
			if (a == b)
			{
				break; // same object
			}

			if (!a->GetCollider() || !b->GetCollider())
			{
				continue; // no collider present
			}

			CollisionManifold manifold = a->GetCollider()->Collides(*b->GetCollider());
			if (manifold.IsColliding)
			{
				collisions.emplace_back(Collision{ a, b, manifold });
			}
		}
	}

	for (SolverBase* solver : m_solvers)
	{
		solver->Solve(collisions, deltaTime);
	}
}
