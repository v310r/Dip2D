#include "Circle.h"
#include "../Collision/CircleCollider.h"

static sf::Texture* g_texture = nullptr;

static void LoadCircleTexture()
{
	g_texture = new sf::Texture();
	if (g_texture)
	{
		g_texture->loadFromFile("assets/textures/Circle.png");
	}
}

Circle::Circle(const glm::vec2 pos, const float r) : Object(pos)
{
	if (!g_texture)
	{
		LoadCircleTexture();
	}

	m_sprite.setTexture(*g_texture);
	m_sprite.setPosition(pos.x, pos.y);
	m_sprite.setOrigin(g_texture->getSize().x / 2.0f, g_texture->getSize().y / 2.0f);

	const float radius = g_texture->getSize().x / 2.0f;
	m_collider.reset(new CircleCollider(pos, radius));


	m_inertia = m_mass * radius * radius / 2.0f;
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
