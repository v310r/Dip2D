#pragma once

#include "Object.h"


class Rectangle : public Object
{
public:
	Rectangle(const glm::vec2 position);

private:
	sf::RectangleShape recShape;
};

