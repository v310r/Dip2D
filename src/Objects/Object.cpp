#include "Object.h"


Object::Object(const glm::vec2 position) : m_position(position)
{

}

void Object::SetPosition(const glm::vec2 position)
{
	m_position = position;
	m_sprite.setPosition(position.x, position.y);
	m_collider->Position = position;
}

void Object::AddToPosition(const glm::vec2 position)
{
	m_sprite.setPosition(m_position.x + position.x, m_position.y + position.y);
	m_position += position;
	m_collider->Position += position;
}

float Object::GetInvMass()
{
	if (m_mass == 0.0f)
	{
		return 0.0f;
	}

	return 1.0f / m_mass;
}

float Object::GetInvInertia()
{
	if (m_inertia == 0.0f)
	{
		return 0.0f;
	}

	return 1.0f / m_inertia;
}

