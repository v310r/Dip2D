#pragma once

#include "../Collision/Collider.h"

class Object
{
public:

	Object(const glm::vec2 position);

	void SetPosition(const glm::vec2 position);
	glm::vec2 GetPosition() { return m_position; }

	void AddToPosition(const glm::vec2 position);

	void SetVelocity(const glm::vec2 velocity) { m_velocity = velocity; }
	glm::vec2 GetVelocity() { return m_velocity; }

	void AddVelocity(const glm::vec2 velocity) { m_velocity += velocity; }

	void ApplyLinearDamping(const float damping) { m_velocity *= damping; }

	void SetNetForce(const glm::vec2 force) { m_netForce = force; }
	glm::vec2 GetNetForce() { return m_netForce; }

	void AddToNetForce(const glm::vec2 force) { m_netForce += force; }

	void SetMass(const float mass) { m_mass = mass; }
	float GetMass() { return m_mass; }

	float GetInvMass();

	void AddLinearImpulse(const glm::vec2 impulse) { m_velocity += impulse; }

	float GetRestitution() { return m_restitution; }

	float GetFriction() { return m_friction; }

	void SetTexture(const sf::Texture texture) { m_texture = texture; }
	sf::Sprite GetSprite() { return m_sprite; }

	void SetCollider(Collider* collider) { m_collider.reset(collider); }
	const Collider* const GetCollider() { return m_collider.get(); }

	void SetDynamic(const bool value) { m_isStatic = !value; }

	bool IsStatic() { return m_isStatic; }
	bool IsDynamic() { return !m_isStatic; }

	virtual void Draw(sf::RenderWindow& window) = 0;

protected:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	std::unique_ptr<Collider> m_collider = nullptr;

	bool m_isStatic = false;

	glm::vec2 m_position{};
	glm::vec2 m_velocity{};
	glm::vec2 m_netForce{};
	float m_mass = 1.0f;
	float m_restitution = 0.5f;
	float m_friction = 0.5f;
};

