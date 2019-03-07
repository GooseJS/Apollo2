#pragma once

#include "Apollo/World/Block.h"
#include "Apollo/World/World.h"
#include "Apollo/World/WorldTypes.h"

namespace Apollo
{
	// TODO (Brendan): TODO List for World Generation
	// - Biomes!
	// - Caves
	// - Ore Gen

	class WorldGenerator
	{
	protected:
		WorldGenerator() {}
		virtual ~WorldGenerator() = default;
	public:
		virtual int getTopBlockHeightAt(int x, int y) = 0;
		virtual Block getBlockAt(BlockPos pos, int& y) = 0; // TODO: This should be turned into more of a "hey this is what the top soil block is, this is what the deep dirt is, this is what stone is, etc."

		virtual void generateChunk(World& world, ChunkPos chunkPos) = 0;
	};
}