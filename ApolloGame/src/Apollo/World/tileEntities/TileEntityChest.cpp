#include "Apollo/World/tileEntities/TileEntityChest.h"

namespace Apollo
{
	TileEntityChest::TileEntityChest(BlockPos pos) : TileEntity(Apollo::BlockManager::getInstance().getBlock(3), pos) // TODO: This is temp code, probably should be staticly referencing a copy?
	{

	}
}