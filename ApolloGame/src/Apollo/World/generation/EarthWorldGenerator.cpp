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

	void EarthWorldGenerator::generateChunk(World& world, int chunkX)
	{
		int firstChunkX = chunkX * APOLLO_CHUNK_WIDTH;
		for (int x = firstChunkX; x < firstChunkX + APOLLO_CHUNK_WIDTH; x++) // World gen pass
		{
			int topBlockHeight = getTopBlockHeightAt(x);
			world.setBlock(BlockPos(x, topBlockHeight), Apollo::BlockManager::getInstance().getBlock(2));	// Note: Setting top height block to grass
			for (int y = 0; y < topBlockHeight; y++)
			{
				world.setBlock(BlockPos(x, y), Apollo::BlockManager::getInstance().getBlock(1));			// Note: Setting all blocks below top Y to dirt
			}
		}
		// Yes, I know these can be done in the same for loop. This is temporary code and it'll be convient to already have these seperated into seperate passes
		for (int x = firstChunkX; x < firstChunkX + APOLLO_CHUNK_WIDTH; x++) // Cave pass
		{
			int topBlockHeight = getTopBlockHeightAt(x) - 20; // Note: buffer between ground level and first possibilty of cave

			for (int y = 10; y < topBlockHeight; y++)
			{
				if (getTopBlockHeightAt(x, y) > 110)
					world.setBlock(BlockPos(x, y), Apollo::BlockManager::getInstance().getBlock(0));
			}
			
		}
	}
}