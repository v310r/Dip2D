#include "Rectangle.h"
#include "../Collision/RectangleCollider.h"
#include "SFML/Graphics/Color.hpp"


Rectangle::Rectangle(const glm::vec2 position, const glm::vec2 size) : Object(position)
{
	m_recShape.setSize({ size.x, size.y });
	m_recShape.setOrigin(m_recShape.getSize().x / 2.0f, m_recShape.getSize().y / 2.0f);
	m_recShape.setPosition(position.x, position.y);
	m_recShape.setFillColor(sf::Color::Magenta);
	const glm::vec2 min = { m_position.x - m_recShape.getSize().x / 2.0f, m_position.y - m_recShape.getSize().y / 2.0f };
	const glm::vec2 max = { m_position.x + m_recShape.getSize().x / 2.0f, m_position.y + m_recShape.getSize().y / 2.0f };

	m_collider.reset(new RectangleCollider(position, min, max));
}

void Rectangle::AddRotationalImpulse(const glm::vec2 point, const glm::vec2 impulse)
{
}

void Rectangle::Draw(sf::RenderWindow& window)
{
	window.draw(m_recShape);
}
