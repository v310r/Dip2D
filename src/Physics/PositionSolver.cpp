#include "PositionSolver.h"
#include "../Objects/Object.h"


void PositionSolver::Solve(const std::vector<Collision>& collisions, float deltaTime)
{
	//  The linear projection value indicates how much positional correction to apply. A 
	//  smaller value will allow objects to penetrate more.Try to keep the value of this
	//	variable between 0.2 and 0.8
	const float LinearProjectionPercent = 0.8f;

	//  The PenetrationSlack determines how much to allow objects to penetrate.This
	//	helps avoid jitter.The larger this number, the less jitter we have in the system.Keep
	//	the value between 0.01 and 0.1
	const float PenetrationSlack = 0.1f;

	for (const Collision& collision : collisions)
	{
		const float invMassA = (collision.A->IsDynamic() ? collision.A->GetInvMass() : 0.0f);
		const float invMassB = (collision.B->IsDynamic() ? collision.B->GetInvMass() : 0.0f);
		const float invMassSum = invMassA + invMassB;
		if (invMassSum == 0.0f)
		{
			continue;
		}

		const float depth = std::fmaxf(collision.Manifold.PenetrationDepth - PenetrationSlack, 0.0f);
		const float scalar = depth / invMassSum;
		const glm::vec2 correction = collision.Manifold.normal * scalar * LinearProjectionPercent;

		if (collision.A->IsDynamic())
		{
			collision.A->AddToPosition(correction * invMassA);
		}
		if (collision.B->IsDynamic())
		{
			collision.B->AddVelocity(-correction * invMassB);
		}
	}
}
