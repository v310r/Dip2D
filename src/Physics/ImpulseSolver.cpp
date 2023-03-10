#include "ImpulseSolver.h"
#include "../Objects/Object.h"
#include "../Collision/CollisionUtility.h"
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

		const glm::vec2 velocityA = (collision.A->IsDynamic() ? collision.A->GetVelocity() : glm::vec2());
		const glm::vec2 velocityB = (collision.B->IsDynamic() ? collision.B->GetVelocity() : glm::vec2());

		const glm::vec2 rA = collision.Manifold.contacts[0] - collision.A->GetPosition();
		const glm::vec2 rB = collision.Manifold.contacts[0] - collision.B->GetPosition();

		const glm::vec3 rA3 = glm::vec3(rA.x, rA.y, 0.0f);
		const glm::vec3 rB3 = glm::vec3(rB.x, rB.y, 0.0f);

		const float inverseInertiaA = (collision.A->IsDynamic() ? collision.A->GetInertia() : 0.0f);
		const float inverseInertiaB = (collision.B->IsDynamic() ? collision.B->GetInertia() : 0.0f);

		const glm::vec2 angularVelocityA = (collision.A->IsDynamic() ? collision.A->GetAngularVelocity() : glm::vec2());
		const glm::vec2 angularVelocityB = (collision.B->IsDynamic() ? collision.B->GetAngularVelocity() : glm::vec2());

		const glm::vec3 angularVelocityA3 = glm::vec3(angularVelocityA.x, angularVelocityA.y, 0.0f);
		const glm::vec3 angularVelocityB3 = glm::vec3(angularVelocityB.x, angularVelocityB.y, 0.0f);


		glm::vec3 crossB3 = glm::cross(angularVelocityB3, rB3);
		glm::vec3 crossA3 = glm::cross(angularVelocityA3, rA3);

		glm::vec2 crossB2 = glm::vec2(crossB3.x, crossB3.y);
		glm::vec2 crossA2 = glm::vec2(crossA3.x, crossA3.y);

		const glm::vec2 relativeVelocity = (velocityB + crossB2) - (velocityA + crossA2);
		const glm::vec2 relativeNormal = collision.Manifold.normal;
		const glm::vec3 relativeNormal3 = glm::vec3(relativeNormal.x, relativeNormal.y, 0.0f);
		const float directionMagnitude = glm::dot(relativeVelocity, relativeNormal);
		if (directionMagnitude <= 0.0f)
		{
			continue; // Moving away from each other? Do nothing!
		}

		const float e = std::fminf(collision.A->GetRestitution(), collision.B->GetRestitution());
		float numerator = (-(1.0f + e) * directionMagnitude);

		float d1 = invMassSum;

		glm::vec3 d2vec3 = glm::cross(glm::cross(rA3, relativeNormal3) * inverseInertiaA, rA3);
		glm::vec2 d2 = glm::vec2(d2vec3.x, d2vec3.y);

		glm::vec3 d3vec3 = glm::cross(glm::cross(rB3, relativeNormal3) * inverseInertiaB, rB3);
		glm::vec2 d3 = glm::vec2(d3vec3.x, d3vec3.y);

		float denominator = d1 + glm::dot(relativeNormal, d2 + d3);

		float j = (denominator == 0.0f) ? 0.0f : numerator / denominator;
		if (collision.Manifold.contacts.size() > 0 && j != 0.0f)
		{
			j /= static_cast<float>(collision.Manifold.contacts.size());
		}

		// Linear Impulse
		const glm::vec2 impulse = relativeNormal * j;
		const glm::vec3 impulse3 = glm::vec3(impulse.x, impulse.y, 0.0f);
		if (collision.A->IsDynamic())
		{
			collision.A->AddVelocity(-impulse * invMassA);

			crossA3 = glm::cross(rA3, impulse3) * inverseInertiaA;
			const glm::vec2 deltaAngularVelocity = glm::vec2(crossA3.x, crossA3.y);
			collision.A->AddAngularVelocity(-deltaAngularVelocity);
		}

		if (collision.B->IsDynamic())
		{
			collision.B->AddVelocity(impulse * invMassB);

			crossB3 = glm::cross(rB3, impulse3) * inverseInertiaB;
			const glm::vec2 deltaAngularVelocity = glm::vec2(crossB3.x, crossB3.y);
			collision.B->AddAngularVelocity(deltaAngularVelocity);
		}

		//Friction
		const glm::vec2 t = relativeVelocity - (relativeNormal * directionMagnitude);
		if (CMP(glm::length(t), 0.0f))
		{
			continue;
		}
		const glm::vec2 tNormalized = glm::normalize(t);
		const glm::vec3 tNormalized3 = glm::vec3(tNormalized.x, tNormalized.y, 0.0f);

		numerator = -glm::dot(relativeVelocity, tNormalized);

		d1 = invMassSum;

		d2vec3 = glm::cross(glm::cross(rA3, tNormalized3) * inverseInertiaA, rA3);
		d2 = glm::vec2(d2vec3.x, d2vec3.y);

		d3vec3 = glm::cross(glm::cross(rB3, tNormalized3) * inverseInertiaB, rB3);
		d3 = glm::vec2(d3vec3.x, d3vec3.y);

		denominator = d1 + glm::dot(tNormalized, d2 + d3);

		if (denominator == 0.0f)
		{
			return;
		}

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
		const glm::vec3 tangentImpulse3 = glm::vec3(tangentImpulse.x, tangentImpulse.y, 0.0f);
		if (collision.A->IsDynamic())
		{
			collision.A->AddVelocity(tangentImpulse * invMassA);

			crossA3 = glm::cross(rA3, tangentImpulse3) * inverseInertiaA;
			const glm::vec2 deltaAngularVelocity = glm::vec2(crossA3.x, crossA3.y);
			collision.A->AddAngularVelocity(deltaAngularVelocity);
		}
		if (collision.B->IsDynamic())
		{
			collision.B->AddVelocity(-tangentImpulse * invMassB);

			crossB3 = glm::cross(rB3, tangentImpulse3) * inverseInertiaB;
			const glm::vec2 deltaAngularVelocity = glm::vec2(crossB3.x, crossB3.y);
			collision.B->AddAngularVelocity(-deltaAngularVelocity);
		}
	}
}
