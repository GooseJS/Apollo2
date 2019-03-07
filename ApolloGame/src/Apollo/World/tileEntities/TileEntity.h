#pragma once

#include "Apollo/World/WorldTypes.h"
#include "Apollo/World/blocks/BlockTickable.h"

namespace Apollo
{
	class TileEntity
	{
	private:
		Block& _parentBlock;
		BlockPos _blockPos;
	public:
		TileEntity(Block& parentBlock, BlockPos blockPos);
		virtual void tick() = 0;
	};
}