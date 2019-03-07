#pragma once

#include <map>
#include <cstdint>

#include <Apollo/Apollo.h>
#include <Apollo/logger/Logger.h>

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
		
		inline ChunkMap& getRenderChunks() { return _chunks; }
	};

	class World
	{
	private:
		ChunkProvider _chunkProvider;
	public:
		void setBlock(BlockPos pos, Block block, TileEntityPtr tileEntity = nullptr);
		Block getBlock(BlockPos pos);
		TileEntityPtr getTileEntity(BlockPos pos);

		ChunkPtr getChunkAt(ChunkPos pos);

		inline ChunkMap& getRenderChunks()
		{
			return _chunkProvider.getRenderChunks();
		}

		inline bool chunkExistsAt(ChunkPos pos) { return _chunkProvider.chunkExistsAt(pos); }
	};
}