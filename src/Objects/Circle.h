#pragma once

#include "Object.h"


class Circle : public Object
{

public:
	Circle(const glm::vec2 pos, const float radius);

	virtual void AddRotationalImpulse(const glm::vec2 point, const glm::vec2 impulse) override;

protected:
	virtual void Draw(sf::RenderWindow& window) override;
};

