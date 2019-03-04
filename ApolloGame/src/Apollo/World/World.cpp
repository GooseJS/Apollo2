#include "Apollo/World/World.h"

namespace Apollo
{
	bool ChunkProvider::chunkExistsAt(ChunkPos pos)
	{
		return _chunks.find(pos.value) != _chunks.end();
	}

	ChunkPtr ChunkProvider::getChunkAt(ChunkPos pos)
	{
		auto chunkIter = _chunks.find(pos.value);
		ChunkPtr chunk;
		if (chunkIter == _chunks.end())
		{
			AP_INFO("Creating chunk at {},{}", pos.x, pos.y);
			// TODO: This is when we attempt to either load chunk from disk if it's been generated, or generate it if it hasn't
			chunk = std::make_shared<Chunk>(pos);
			// TODO: Chunk initialize would / will happen here?
			_chunks.insert(std::pair<uint64_t, ChunkPtr>(pos.value, chunk));
		}
		else
		{
			chunk = chunkIter->second;
		}

		return chunk;
	}

	void World::setBlock(BlockPos pos, Block block)
	{
		_chunkProvider.getChunkAt(pos)->setBlock(pos, block);
	}
	
	Block World::getBlock(BlockPos pos)
	{
		return _chunkProvider.getChunkAt(pos)->getBlock(pos);
	}

	ChunkPtr World::getChunkAt(ChunkPos pos)
	{
		return _chunkProvider.getChunkAt(pos);
	}
}