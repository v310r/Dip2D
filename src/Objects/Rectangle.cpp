#include "Rectangle.h"
#include "../Collision/RectangleCollider.h"

Rectangle::Rectangle(const glm::vec2 position) : Object(position)
{
	recShape.setSize({ 300.0f, 40.0f });
	const glm::vec2 min = { m_position.x - recShape.getSize().x, m_position.y - recShape.getSize().y };
	const glm::vec2 max = { m_position.x + recShape.getSize().x, m_position.y + recShape.getSize().y };
	Collider* collider = new RectangleCollider(position, min, max);
}