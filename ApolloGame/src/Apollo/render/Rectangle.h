#pragma once

#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <Apollo/render/2d/DebugRenderer.h>

#include "Apollo/World/WorldTypes.h"

namespace Apollo
{
	class Rectangle
	{
	private:
		float _x = 0;
		float _y = 0;
		float _width = 0;
		float _height = 0;

	public:
		Rectangle();
		Rectangle(glm::vec4 bounds);
		Rectangle(float x, float y, float width, float height);

		bool overlaps(const Rectangle& rectangle);

		std::vector<BlockPos> getContainingBlockPos();

		inline glm::vec4 getContainingBounds() { return glm::vec4(_x, _y, _width, _height); }

		inline float getX() { return _x; }
		inline float getY() { return _y; }
		inline float getWidth() { return _width; }
		inline float getHeight() { return _height; }

		inline void setPos(float x, float y) { _x = x; _y = y; }
		inline void setWidth(float width) { _width = width; }
		inline void setHeight(float height) { _height = height; }

		operator DebugRenderer::Rectangle()
		{
			return DebugRenderer::Rectangle(_x, _y, _width, _height);
		}
	};
}