#include "Circle.h"
#include "../Collision/CircleCollider.h"

Circle::Circle(const glm::vec2 pos, const float radius) : Object(pos)
{
	m_texture.loadFromFile("assets/textures/Circle.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(pos.x, pos.y);
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f, m_texture.getSize().y / 2.0f);
	m_collider.reset(new CircleCollider(pos, m_texture.getSize().x / 2.0f));
}

void Circle::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}
