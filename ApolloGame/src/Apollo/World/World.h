#pragma once

#include <map>
#include <cstdint>

#include <Apollo/Apollo.h>
#include <Apollo/logger/Logger.h>
#include <Apollo/render/2d/OrthographicCamera.h>

#include "Apollo/World/WorldTypes.h"
#include "Apollo/World/Block.h"
#include "Apollo/World/Chunk.h"
#include "Apollo/World/tileEntities/TileEntity.h"

namespace Apollo
{
	typedef std::shared_ptr<Chunk> ChunkPtr;
	typedef std::map<uint64_t, ChunkPtr> ChunkMap;

	class ChunkProvider
	{
	private:
		ChunkMap _chunks;
	public:
		bool chunkExistsAt(ChunkPos pos);
		ChunkPtr getChunkAt(ChunkPos pos);
		
		inline ChunkMap& getChunks() { return _chunks; }
	};

	class World
	{
	private:
		ChunkProvider _chunkProvider;

		int _chunkRenderBuffer = 1;
	public:
		Block setBlock(BlockPos pos, Block block, TileEntityPtr tileEntity = nullptr);
		Block getBlock(BlockPos pos);
		TileEntityPtr getTileEntity(BlockPos pos);

		ChunkPtr getChunkAt(ChunkPos pos);

		void tick(BlockPos playerPos);

		inline ChunkMap& getChunks()
		{
			return _chunkProvider.getChunks();
		}

		inline bool chunkExistsAt(ChunkPos pos) { return _chunkProvider.chunkExistsAt(pos); }
	};
}