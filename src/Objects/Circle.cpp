#include "Circle.h"
#include "../Collision/CircleCollider.h"

Circle::Circle(const glm::vec2 pos, const float r) : Object(pos)
{
	m_texture.loadFromFile("assets/textures/Circle.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(pos.x, pos.y);
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f, m_texture.getSize().y / 2.0f);

	const float radius = m_texture.getSize().x / 2.0f;
	m_collider.reset(new CircleCollider(pos, radius));


	m_inertia = m_mass * radius * radius / 2.0f;

	int x = 5;
}

void Circle::AddRotationalImpulse(const glm::vec2 point, const glm::vec2 impulse)
{
	//const glm::vec2 centerOfMass = m_position;
	//const glm::vec2 torque = glm::cross(point - centerOfMass, impulse);
	//const glm::vec2 angularAcceleration = torque * GetInvInertia();
	//m_angularVelocity += angularAcceleration;
}

void Circle::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}
