#pragma once

#include "Object.h"


class Rectangle : public Object
{
public:
	Rectangle(const glm::vec2 position);

protected:
	virtual void Draw(sf::RenderWindow& window) override;

private:
	sf::RectangleShape m_recShape;
};

