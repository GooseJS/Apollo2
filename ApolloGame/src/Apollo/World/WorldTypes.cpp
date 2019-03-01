#include "Apollo/World/WorldTypes.h"

namespace Apollo
{
	BlockPos::BlockPos(const LocalBlockPos& localBlockPos)
	{
		this->x = (APOLLO_CHUNK_WIDTH * localBlockPos.chunkPos.x) + localBlockPos.x;
		this->y = (APOLLO_CHUNK_WIDTH * localBlockPos.chunkPos.y) + localBlockPos.y;
	}
}