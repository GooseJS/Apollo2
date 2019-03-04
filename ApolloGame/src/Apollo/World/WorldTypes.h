#pragma once

#include <cstdint>

#ifndef APOLLO_CHUNK_WIDTH
#define APOLLO_CHUNK_WIDTH 16
#endif

#define APOLLO_CHUNK_AREA APOLLO_CHUNK_WIDTH * APOLLO_CHUNK_WIDTH

#ifndef APOLLO_BLOCK_WIDTH
#define APOLLO_BLOCK_WIDTH 16.0f
#endif

namespace Apollo
{
	struct BlockPos
	{
		int x;
		int y;

		BlockPos() : x(0), y(0) {}
		BlockPos(int x, int y) { this->x = x; this->y = y; }

		BlockPos(const struct LocalBlockPos& localBlockPos);
		//BlockPos(struct ChunkPos chunkPos);

		inline BlockPos& changeX(int changeInX)
		{
			x += changeInX;
			return (*this);
		}

		inline BlockPos& changeY(int changeInY)
		{
			y += changeInY;
			return (*this);
		}
		
		static BlockPos fromScreenCoordinates(float x, float y);
	};

	struct ChunkPos
	{
		union
		{
			struct
			{
				uint32_t x;
				uint32_t y;
			};
			uint64_t value;
		};

		ChunkPos() : x(0), y(0) {}
		ChunkPos(uint64_t value) { this->value = value; }
		ChunkPos(int x, int y) { this->x = x; this->y = y; }

		ChunkPos(const BlockPos& blockPos)
		{
			x = blockPos.x >> 4;
			y = blockPos.y >> 4;
		}
	};

	struct LocalBlockPos
	{
		int x;
		int y;

		ChunkPos chunkPos;

		LocalBlockPos() : x(0), y(0) {}
		LocalBlockPos(int x, int y) { this->x = x; this->y = y; }

		LocalBlockPos(const BlockPos& blockPos)
		{
			x = blockPos.x & APOLLO_CHUNK_WIDTH - 1;
			y = blockPos.y & APOLLO_CHUNK_WIDTH - 1;
			chunkPos = ChunkPos(blockPos);
		}

		const int index()
		{
			return x | y << 4;
		}
	};
}