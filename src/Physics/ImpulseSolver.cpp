#include "ImpulseSolver.h"
#include "../Objects/Object.h"
#include "../Collision/CollisionUtility.h"
#include <cmath>

void ImpulseSolver::Solve(std::vector<Collision> collisions, float deltaTime)
{
	for (const Collision& collision : collisions)
	{
		const float invMassA = (collision.A->IsDynamic() ? collision.A->GetInvMass() : 0.0f);
		const float invMassB = (collision.B->IsDynamic() ? collision.B->GetInvMass() : 0.0f);
		const float invMassSum = invMassA + invMassB;
		if (invMassSum == 0.0f)
		{
			continue;
		}

		const glm::vec2 velocityA = (collision.A->IsDynamic() ? collision.A->GetVelocity() : glm::vec2());
		const glm::vec2 velocityB = (collision.B->IsDynamic() ? collision.B->GetVelocity() : glm::vec2());

		const glm::vec2 relativeVelocity = velocityB - velocityA;
		const glm::vec2 relativeNormal = glm::normalize(collision.Manifold.normal);
		if (glm::dot(relativeVelocity, relativeNormal) > 0.0f)
		{
			continue; // Moving away from each other? Do nothing!
		}

		const float e = std::fminf(collision.A->GetRestitution(), collision.B->GetRestitution());
		float numerator = (-(1.0f + e) * glm::dot(relativeVelocity, relativeNormal));
		float j = numerator / invMassSum;
		if (collision.Manifold.contacts.size() > 0 && j != 0.0f)
		{
			j /= static_cast<float>(collision.Manifold.contacts.size());
		}

		// Linear Impulse
		const glm::vec2 impulse = relativeNormal * j;
		if (collision.A->IsDynamic())
		{
			collision.A->AddVelocity(-impulse * invMassA);
		}
		if (collision.B->IsDynamic())
		{
			collision.B->AddVelocity(impulse * invMassB);
		}

		//Friction
		const glm::vec2 t = relativeVelocity - (relativeNormal * glm::dot(relativeVelocity, relativeNormal));
		if (CMP(glm::length(t), 0.0f))
		{
			return;
		}
		const glm::vec2 tNormalized = glm::normalize(t);

		numerator = -glm::dot(relativeVelocity, t);
		float jt = numerator / invMassSum;
		if (collision.Manifold.contacts.size() > 0 && jt != 0.0f)
		{
			jt /= static_cast<float>(collision.Manifold.contacts.size());
		}

		if (CMP(jt, 0.0f))
		{
			return;
		}

		const float friction = std::sqrtf(collision.A->GetFriction() * collision.B->GetFriction());
		if (jt > j * friction)
		{
			jt = j * friction;
		}
		else if (jt < -j * friction)
		{
			jt = -j * friction;
		}

		const glm::vec2 tangentImpulse = t * jt;
		if (collision.A->IsDynamic())
		{
			collision.A->AddVelocity(-tangentImpulse * invMassA);
		}
		if (collision.B->IsDynamic())
		{
			collision.B->AddVelocity(tangentImpulse * invMassB);
		}
	}
}
