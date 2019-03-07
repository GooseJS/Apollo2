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
}