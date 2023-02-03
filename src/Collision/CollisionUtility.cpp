#include <cmath>
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "CollisionUtility.h"


glm::vec2 CollisionUtility::FindCollision(const RectangleCollider& rectangle, const CircleCollider& circle)
{
	const float halfWidth = (rectangle.Max.x - rectangle.Min.x) / 2;
	const float halfHeight = (rectangle.Max.y - rectangle.Min.y) / 2;

	const glm::vec2 absolutDistance = circle.Position - rectangle.Position;
	const float clampedX = std::clamp(absolutDistance.x, -halfWidth, halfWidth);
	const float clampedY = std::clamp(absolutDistance.y, -halfHeight, halfHeight);
	const glm::vec2 recSurfacePoint = rectangle.Position + glm::vec2{ clampedX , clampedX };

	const float distance = std::sqrt(std::powf(circle.Position.x - recSurfacePoint.x, 2)
		+ std::powf(circle.Position.y - recSurfacePoint.y, 2));	

	if (distance <= circle.Radius)
	{
		return recSurfacePoint;
	}

	return NO_COLLISION;

	//sf::Vector2f recSide;
	//if (sphere.Position.x < rectangle.Min.x) recSide.x = rectangle.Min.x;
	//else if (sphere.Position.x >= rectangle.Max.x) recSide.x = rectangle.Max.x;

	//if (sphere.Position.y < rectangle.Min.y) recSide.y = rectangle.Min.y;
	//else if (sphere.Position.y >= rectangle.Max.y) recSide.y = rectangle.Max.y;

	//const float distance = std::sqrt(std::powf(sphere.Position.x - recSide.x, 2)
	//	+ std::powf(sphere.Position.y - recSide.y, 2));


	//return distance <= sphere.Radius;
}

glm::vec2 CollisionUtility::FindCollision(const RectangleCollider& rectangle1, const RectangleCollider& rectangle2)
{
	//if (rectangle1.Min.x <= rectangle2.Max.x &&
	//	rectangle1.Max.x >= rectangle2.Min.x &&
	//	rectangle1.Min.y <= rectangle2.Max.y &&
	//	rectangle1.Max.y >= rectangle2.Min.y)
	//{

	//}

	return NO_COLLISION;
}

glm::vec2 CollisionUtility::FindCollision(const CircleCollider& sphere1, const CircleCollider& sphere2)
{
	const float distance = std::sqrt(std::powf(sphere1.Position.x - sphere2.Position.x, 2)
		+ std::powf(sphere2.Position.y - sphere2.Position.y, 2));

	if (distance <= sphere1.Radius + sphere2.Radius)
	{
		return glm::vec2{ (sphere1.Position - sphere2.Position) / 2.0f };
	}

	return NO_COLLISION;
}
