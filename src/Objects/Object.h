#pragma once

#include "../Collision/Collider.h"

class Object
{
public:

	Object(const glm::vec2 position);

	void SetPosition(const glm::vec2 position) 
	{ 
		m_position = position; 
		m_sprite.setPosition(position.x, position.y);
	}
	glm::vec2 GetPosition() { return m_position; }

	void SetVelocity(const glm::vec2 velocity) { m_velocity = velocity; }
	glm::vec2 GetVelocity() { return m_velocity; }

	void SetForce(const glm::vec2 force) { m_force = force; }
	glm::vec2 GetForce() { return m_force; }

	void SetMass(const float mass) { m_mass = mass; }
	float GetMass() { return m_mass; }

	void SetTexture(const sf::Texture texture) { m_texture = texture; }
	sf::Sprite GetSprite() { return m_sprite; }

	void SetCollider(Collider* collider) { m_collider.reset(collider); }
	const Collider* const GetCollider() { return m_collider.get(); }


	virtual void Draw(sf::RenderWindow& window) = 0;

protected:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	std::unique_ptr<Collider> m_collider = nullptr;

	glm::vec2 m_position{};
	glm::vec2 m_velocity{};
	glm::vec2 m_force{};
	float m_mass = 1.0f;
};

