#pragma once

#include <FastNoise.h>

#include "Apollo/World/generation/WorldGenerator.h"

namespace Apollo
{
	class EarthWorldGenerator : public WorldGenerator
	{
	private:
		FastNoise noiseGenerator;
		int _worldHeight;
	public:
		EarthWorldGenerator(int worldHeight);
		~EarthWorldGenerator();

		virtual int getTopBlockHeightAt(int x, int y = 0) override;
		virtual Block getBlockAt(BlockPos pos, int& y) override;
	};
}