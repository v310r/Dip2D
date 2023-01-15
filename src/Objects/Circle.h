#pragma once

#include "Object.h"


class Circle : public Object
{

public:
	Circle(const glm::vec2 pos, const float radius);

protected:
	virtual void Draw(sf::RenderWindow& window) override;
};

