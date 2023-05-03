#include "ImpulseSolver.h"
#include "../Objects/Object.h"
#include "../Collision/CollisionUtility.h"
#include <cmath>
//#include <mutex>


//static std::mutex impulseMutex;

void ImpulseSolver::Solve(const std::vector<Collision>& collisions, float deltaTime)
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

		glm::vec2 velocityA;
		glm::vec2 velocityB;

		{
			//std::unique_lock lock(impulseMutex);

			{
				std::unique_lock lock(collision.A->internalMutex);
				velocityA = (collision.A->IsDynamic() ? collision.A->GetVelocity() : glm::vec2());
			}

			{
				std::unique_lock lock(collision.B->internalMutex);
				velocityB = (collision.B->IsDynamic() ? collision.B->GetVelocity() : glm::vec2());
			}
		}

		glm::vec2 relativeVelocity = velocityB - velocityA;
		glm::vec2 relativeNormal = collision.Manifold.normal;
		float directionMagnitude = glm::dot(relativeVelocity, relativeNormal);
		if (directionMagnitude <= 0.0f)
		{
			continue; // Moving away from each other? Do nothing!
		}

		const float e = std::fminf(collision.A->GetRestitution(), collision.B->GetRestitution());
		float numerator = (-(1.0f + e) * directionMagnitude);
		float j = numerator / invMassSum;
		if (collision.Manifold.contacts.size() > 0 && j != 0.0f)
		{
			j /= static_cast<float>(collision.Manifold.contacts.size());
		}

		// Linear Impulse
		const glm::vec2 impulse = relativeNormal * j;

		{
			//std::unique_lock lock(impulseMutex);

			if (collision.A->IsDynamic())
			{
				std::unique_lock lock(collision.A->internalMutex);
				collision.A->AddVelocity(-impulse * invMassA);
			}

			if (collision.B->IsDynamic())
			{
				std::unique_lock lock(collision.B->internalMutex);
				collision.B->AddVelocity(impulse * invMassB);
			}
		}

		//Friction
		{
			//std::unique_lock lock(impulseMutex);

			{
				std::unique_lock lock(collision.A->internalMutex);
				velocityA = (collision.A->IsDynamic() ? collision.A->GetVelocity() : glm::vec2());
			}

			{
				std::unique_lock lock(collision.B->internalMutex);
				velocityB = (collision.B->IsDynamic() ? collision.B->GetVelocity() : glm::vec2());
			}
		}

		relativeVelocity = velocityB - velocityA;
		relativeNormal = collision.Manifold.normal;
		directionMagnitude = glm::dot(relativeVelocity, relativeNormal);

		const glm::vec2 t = relativeVelocity - (relativeNormal * directionMagnitude);
		if (CMP(glm::length(t), 0.0f))
		{
			continue;
		}
		const glm::vec2 tNormalized = glm::normalize(t);

		numerator = -glm::dot(relativeVelocity, tNormalized);
		float jt = numerator / invMassSum;
		if (collision.Manifold.contacts.size() > 0 && jt != 0.0f)
		{
			jt /= static_cast<float>(collision.Manifold.contacts.size());
		}

		if (CMP(jt, 0.0f))
		{
			continue;
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

		const glm::vec2 tangentImpulse = tNormalized * jt;

		{
			//std::unique_lock lock(impulseMutex);

			if (collision.A->IsDynamic())
			{
				std::unique_lock lock(collision.A->internalMutex);
				collision.A->AddVelocity(tangentImpulse * invMassA);
			}

			if (collision.B->IsDynamic())
			{
				std::unique_lock lock(collision.B->internalMutex);
				collision.B->AddVelocity(-tangentImpulse * invMassB);
			}
		}
	}
}
