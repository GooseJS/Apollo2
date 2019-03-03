#include "Apollo/render/Rectangle.h"

namespace Apollo
{
	Rectangle::Rectangle()
	{

	}

	Rectangle::Rectangle(glm::vec4 bounds)
	{
		_x = bounds[0];
		_y = bounds[1];
		_width = bounds[2];
		_height = bounds[3];
	}

	Rectangle::Rectangle(float x, float y, float width, float height) : _x(x), _y(y), _width(width), _height(height)
	{

	}

	bool Rectangle::overlaps(const Rectangle & rectangle)
	{
		// TODO: I need to write this function
		return false;
	}

	std::vector<BlockPos> Rectangle::getContainingBlockPos()
	{
		std::vector<BlockPos> foundPos;

		glm::vec2 lowerLeft = glm::vec2(floor(_x / APOLLO_BLOCK_WIDTH), floor(_y / APOLLO_BLOCK_WIDTH));
		int blockWidth = ceil((_x + _width) / APOLLO_BLOCK_WIDTH) - lowerLeft.x;
		int blockHeight = ceil((_y + _height) / APOLLO_BLOCK_WIDTH) - lowerLeft.y;

		float buffer = 1.f / (APOLLO_BLOCK_WIDTH / 2.f);

		if (((_x + _width) / APOLLO_BLOCK_WIDTH) > lowerLeft.x + blockWidth) blockWidth++;
		if (((_y + _height) / APOLLO_BLOCK_WIDTH) + buffer > lowerLeft.y + blockHeight) blockHeight++;

		for (int x = lowerLeft.x; x < lowerLeft.x + blockWidth; x++)
		{
			for (int y = lowerLeft.y; y < lowerLeft.y + blockHeight; y++)
			{
				foundPos.emplace_back(x, y);
			}
		}

		return foundPos;
	}
}