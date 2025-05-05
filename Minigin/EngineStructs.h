#pragma once

#include <glm.hpp>

namespace Engine
{
	struct Rect
	{
		Rect() = default;

		Rect(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {
		}

		Rect(glm::vec2 position, float width, float height)
			: x(position.x), y(position.y), width(width), height(height) {
		}

		float x, y;
		float width, height;
	};
}