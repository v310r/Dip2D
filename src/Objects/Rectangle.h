#pragma once

#include "Object.h"


class Rectangle : public Object
{
public:
	Rectangle(const glm::vec2 position, const glm::vec2 size = { 300.0f, 40.0f });

	virtual void AddRotationalImpulse(const glm::vec2 point, const glm::vec2 impulse) override;

protected:
	virtual void Draw(sf::RenderWindow& window) override;

private:
	sf::RectangleShape m_recShape;
};

