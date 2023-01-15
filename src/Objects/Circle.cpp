#include "Circle.h"
#include "../Collision/CircleCollider.h"

Circle::Circle(const glm::vec2 pos, const float radius) : Object(pos)
{
	m_texture.loadFromFile("assets/textures/Circle.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(pos.x, pos.y);
	Collider* collider = new CircleCollider(pos, radius);
}

void Circle::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}
