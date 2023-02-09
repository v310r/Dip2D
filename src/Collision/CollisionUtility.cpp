#include <cmath>
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "CollisionUtility.h"

inline bool CMP(const float x, const float y)
{
	return fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y)));
}

CollisionManifold CollisionUtility::GetCollisionManifold(const RectangleCollider& rectangle, const CircleCollider& circle)
{
	CollisionManifold result;

	const float halfWidth = (rectangle.Max.x - rectangle.Min.x) / 2;
	const float halfHeight = (rectangle.Max.y - rectangle.Min.y) / 2;

	const glm::vec2 absolutDistance = circle.Position - rectangle.Position;
	const float clampedX = std::clamp(absolutDistance.x, -halfWidth, halfWidth);
	const float clampedY = std::clamp(absolutDistance.y, -halfHeight, halfHeight);
	const glm::vec2 closestPoint = rectangle.Position + glm::vec2{ clampedX , clampedY };

	const glm::vec2 distanceVec = circle.Position - closestPoint;
	const float distance = glm::length(distanceVec);

	if (distance > circle.Radius)
	{
		return result;
	}

	glm::vec2 normal;
	if (CMP(distance, 0.0f))
	{
		const glm::vec2 revertedDistanceVec = closestPoint - rectangle.Position;
		const float newDistance = glm::length(revertedDistanceVec);
		if (CMP(newDistance, 0.0f))
		{
			return result;
		}

		normal = glm::normalize(revertedDistanceVec);
	}
	else
	{
		normal = glm::normalize(distanceVec);
	}

	const glm::vec2 outsidePoint = circle.Position - normal * circle.Radius;
	const float doublePenetrationDistance = glm::length(closestPoint - outsidePoint);

	result.IsColliding = true;
	result.contacts.emplace_back(closestPoint + (outsidePoint - closestPoint) * 0.5f);
	result.normal = normal;
	result.PenetrationDepth = doublePenetrationDistance * 0.5f;

	return result;
}

CollisionManifold CollisionUtility::GetCollisionManifold(const RectangleCollider& rectangle1, const RectangleCollider& rectangle2)
{
	//if (rectangle1.Min.x <= rectangle2.Max.x &&
	//	rectangle1.Max.x >= rectangle2.Min.x &&
	//	rectangle1.Min.y <= rectangle2.Max.y &&
	//	rectangle1.Max.y >= rectangle2.Min.y)
	//{

	//}

	return CollisionManifold();
}

CollisionManifold CollisionUtility::GetCollisionManifold(const CircleCollider& circle1, const CircleCollider& circle2)
{
	CollisionManifold result;

	const glm::vec2 distanceVec = (circle2.Position - circle1.Position);
	const float combinedRadius = circle1.Radius + circle2.Radius;
	const float distance = glm::length(distanceVec);
	if (distance > combinedRadius || distance == 0.0f)
	{
		return result;
	}

	const glm::vec2 normal = glm::normalize(distanceVec);

	result.normal = normal;
	result.IsColliding = true;
	result.PenetrationDepth = (distance - circle1.Radius + circle2.Radius) * 0.5f;

	const float distanceToIntersectionPoint = circle1.Radius - result.PenetrationDepth;
	const glm::vec2 contactPoint = circle1.Position + result.normal * distanceToIntersectionPoint;
	result.contacts.emplace_back(contactPoint);

	return result;
}
