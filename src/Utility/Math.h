#pragma once

#include "glm/glm.hpp"

namespace Math
{
	float Cross2D(const glm::vec2 & a, const glm::vec2& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	glm::vec2 Cross2D(const glm::vec2& a, float s)
	{
		return glm::vec2(s * a.y, -s * a.x);
	}

	glm::vec2 Cross2D(float s, const glm::vec2& a)
	{
		return glm::vec2(-s * a.y, s * a.x);
	}
}