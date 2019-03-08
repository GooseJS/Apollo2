#include "Apollo/World/WorldTypes.h"

namespace Apollo
{
	// TODO: Fucking allow more conversations between ChunkPos -> BlockPos, etc
	BlockPos BlockPos::fromScreenCoordinates(float x, float y)
	{
		return BlockPos(x / APOLLO_BLOCK_WIDTH, y / APOLLO_BLOCK_WIDTH);
	}

	BlockPos::BlockPos(const LocalBlockPos& localBlockPos)
	{
		this->x = (APOLLO_CHUNK_WIDTH * localBlockPos.chunkPos.x) + localBlockPos.x;
		this->y = (APOLLO_CHUNK_WIDTH * localBlockPos.chunkPos.y) + localBlockPos.y;
	}
}