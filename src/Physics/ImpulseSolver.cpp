#include "ImpulseSolver.h"
#include "../Objects/Object.h"
#include "../Collision/CollisionUtility.h"
#include "../Utility/Math.h"
#include <cmath>


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

		glm::vec2 velocityA = (collision.A->IsDynamic() ? collision.A->GetVelocity() : glm::vec2());
		glm::vec2 velocityB = (collision.B->IsDynamic() ? collision.B->GetVelocity() : glm::vec2());

		const glm::vec2 rA = collision.Manifold.contacts[0] - collision.A->GetPosition();
		const glm::vec2 rB = collision.Manifold.contacts[0] - collision.B->GetPosition();

		const float inverseInertiaA = (collision.A->IsDynamic() ? collision.A->GetInvInertia() : 0.0f);
		const float inverseInertiaB = (collision.B->IsDynamic() ? collision.B->GetInvInertia() : 0.0f);

		const float angularVelocityA = (collision.A->IsDynamic() ? collision.A->GetAngularVelocity() : 0.0f);
		const float angularVelocityB = (collision.B->IsDynamic() ? collision.B->GetAngularVelocity() : 0.0f);

		glm::vec2 relativeVelocity = (velocityB + Math::Cross2D(angularVelocityB, rB)) - 
										   (velocityA + Math::Cross2D(angularVelocityA, rA));

		const glm::vec2 relativeNormal = collision.Manifold.normal;
		float directionMagnitude = glm::dot(relativeVelocity, relativeNormal);
		if (directionMagnitude <= 0.0f)
		{
			continue; // Moving away from each other? Do nothing!
		}

		const float e = std::fminf(collision.A->GetRestitution(), collision.B->GetRestitution());
		float numerator = (-(1.0f + e) * directionMagnitude);

		float d1 = invMassSum;
		glm::vec2 d2 = Math::Cross2D(Math::Cross2D(rA, relativeNormal) * inverseInertiaA, rA);
		glm::vec2 d3 = Math::Cross2D(Math::Cross2D(rB, relativeNormal) * inverseInertiaB, rB);

		float denominator = d1 + glm::dot(relativeNormal, d2 + d3);
		float j = (denominator == 0.0f) ? 0.0f : numerator / denominator;

		if (collision.Manifold.contacts.size() > 0 && j != 0.0f)
		{
			j /= static_cast<float>(collision.Manifold.contacts.size());
		}

		// Linear Impulse
		const glm::vec2 impulse = relativeNormal * j;
		if (collision.A->IsDynamic())
		{
			collision.A->AddVelocity(-impulse * invMassA);
			collision.A->AddAngularVelocity(-Math::Cross2D(rA, impulse) * inverseInertiaA);
		}

		if (collision.B->IsDynamic())
		{
			collision.B->AddVelocity(impulse * invMassB);
			collision.B->AddAngularVelocity(Math::Cross2D(rB, impulse) * inverseInertiaB);
		}

		//Friction
		velocityA = (collision.A->IsDynamic() ? collision.A->GetVelocity() : glm::vec2());
		velocityB = (collision.B->IsDynamic() ? collision.B->GetVelocity() : glm::vec2());

		relativeVelocity = (velocityB + Math::Cross2D(angularVelocityB, rB)) - (velocityA + Math::Cross2D(angularVelocityA, rA));
		directionMagnitude = glm::dot(relativeVelocity, relativeNormal);

		const glm::vec2 t = relativeVelocity - (relativeNormal * directionMagnitude);
		if (CMP(glm::length(t), 0.0f))
		{
			continue;
		}

		const glm::vec2 tNormalized = glm::normalize(t);
		d1 = invMassSum;
		d2 = Math::Cross2D(Math::Cross2D(rA, tNormalized) * inverseInertiaA, rA);
		d3 = Math::Cross2D(Math::Cross2D(rB, tNormalized) * inverseInertiaB, rB);

		denominator = d1 + glm::dot(tNormalized, d2 + d3);
		if (denominator == 0.0f)
		{
			continue;
		}

		numerator = -glm::dot(relativeVelocity, tNormalized);
		float jt = numerator / denominator;
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
		if (collision.A->IsDynamic())
		{
			collision.A->AddVelocity(tangentImpulse * invMassA);
			collision.A->AddAngularVelocity(Math::Cross2D(rA, tangentImpulse) * inverseInertiaA);
		}

		if (collision.B->IsDynamic())
		{
			collision.B->AddVelocity(-tangentImpulse * invMassB);
			collision.B->AddAngularVelocity(-Math::Cross2D(rB, tangentImpulse) * inverseInertiaB);
		}
	}
}
