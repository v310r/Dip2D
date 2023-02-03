#include "PhysicsWorld.h"

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

void PhysicsWorld::Step(const float deltaTime)
{
	//if (m_objectsRef.size() == 0) return;

	for (Object* const obj : m_objectsRef)
	{
		obj->SetForce(obj->GetForce() + obj->GetMass() * m_gravity);

		obj->SetVelocity(obj->GetVelocity() + obj->GetForce() / obj->GetMass() * deltaTime);
		obj->SetPosition(obj->GetPosition() + obj->GetVelocity());

		obj->SetForce({ 0.0f, 0.0f });
	}
}

void PhysicsWorld::ResolveCollisions(const float deltaTime)
{
	std::vector<Manifold> collisions;
	for (Object* const x : m_objectsRef)
	{
		for (Object* const y : m_objectsRef)
		{
			if (x == y)
			{
				break; // same object
			}

			if (!x->GetCollider() || !y->GetCollider())
			{
				continue; // no collider present
			}

			glm::vec2 collisionPoint = x->GetCollider()->Collides(*y->GetCollider());
			if (collisionPoint != NO_COLLISION)
			{
				collisions.emplace_back(Manifold{ x, y, 0.0f, collisionPoint });
			}
		}
	}

	for (SolverBase* SolverBase : m_SolverBases)
	{
		SolverBase->Solve(collisions, deltaTime);
	}
}
