#pragma once

#include "Apollo/World/Block.h"
#include "Apollo/World/WorldTypes.h"

namespace Apollo
{
	class WorldGenerator
	{
	protected:
		WorldGenerator() {}
		virtual ~WorldGenerator() = default;
	public:
		virtual int getTopBlockHeightAt(int x, int y) = 0;
		virtual Block getBlockAt(BlockPos pos, int& y) = 0;
	};
}