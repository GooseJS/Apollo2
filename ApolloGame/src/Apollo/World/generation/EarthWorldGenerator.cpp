#include "Apollo/World/generation/EarthWorldGenerator.h"

namespace Apollo
{
	EarthWorldGenerator::EarthWorldGenerator(int worldHeight) : _worldHeight(worldHeight)
	{
		noiseGenerator.SetNoiseType(FastNoise::SimplexFractal);
		//noiseGenerator.SetFractalGain(0.01);
		noiseGenerator.SetFrequency(0.01);
	}

	EarthWorldGenerator::~EarthWorldGenerator()
	{

	}

	int EarthWorldGenerator::getTopBlockHeightAt(int x, int y)
	{
		return (noiseGenerator.GetNoise(x, y) + 1.0f) * ((float)_worldHeight / 2.0f);
	}

	Block EarthWorldGenerator::getBlockAt(BlockPos pos, int& y)
	{
		return BlockManager::getInstance().getBlock(0);
	}

	void EarthWorldGenerator::generateChunk(World& world, ChunkPos chunkPos)
	{
		ChunkPtr chunk = world.getChunkAt(chunkPos);
		int blockX = chunkPos.x * APOLLO_CHUNK_WIDTH;
		int blockY = chunkPos.y * APOLLO_CHUNK_WIDTH; // TODO: Fucking allow more conversations between ChunkPos -> BlockPos, etc
		for (int x = 0; x < APOLLO_CHUNK_WIDTH; x++)
		{
			int topBlockHeight = getTopBlockHeightAt(blockX, blockY);
			if (topBlockHeight > blockY)
				continue;
			if (topBlockHeight <= blockY + APOLLO_CHUNK_WIDTH)
			{
				chunk->setBlock(LocalBlockPos(x, topBlockHeight), Apollo::BlockManager::getInstance().getBlock(2)); // Note: Setting top height block to grass
				for (int y = topBlockHeight; y >= blockY; y--)
				{
					chunk->setBlock(LocalBlockPos(x, y), Apollo::BlockManager::getInstance().getBlock(1)); // Note: Setting all blocks below top Y to dirt
				}
			}
			else
			{
				for (int y = 0; y < APOLLO_CHUNK_WIDTH; y++)
				{
					chunk->setBlock(LocalBlockPos(x, y), Apollo::BlockManager::getInstance().getBlock(1)); // Note: Setting everything to dirt because the top Y is larger than our top Y
				}
			}
		}
	}
}