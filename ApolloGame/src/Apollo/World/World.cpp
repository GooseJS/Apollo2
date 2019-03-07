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

	Block World::setBlock(BlockPos pos, Block block, TileEntityPtr tileEntity)
	{
		ChunkPtr currentChunk = _chunkProvider.getChunkAt(pos);
		Block lastBlock = currentChunk->getBlock(pos);
		if (lastBlock.getData().isTileEntityBlock)
			currentChunk->cleanTileEntity(pos);
		currentChunk->setBlock(pos, block, tileEntity);
		if (block.getData().isTileEntityBlock && tileEntity == nullptr)
			AP_CRITICAL("Technically an error. Trying to set a block that should be a tile entity without giving it a tile entity");
		return lastBlock;
	}
	
	Block World::getBlock(BlockPos pos)
	{
		Block block = _chunkProvider.getChunkAt(pos)->getBlock(pos);
		return block;
	}

	TileEntityPtr World::getTileEntity(BlockPos pos)
	{
		TileEntityPtr tileEntity = _chunkProvider.getChunkAt(pos)->getTileEntity(pos);
		return tileEntity;
	}

	ChunkPtr World::getChunkAt(ChunkPos pos)
	{
		return _chunkProvider.getChunkAt(pos);
	}

	void World::tick(BlockPos playerPos)
	{
		for (auto iter = _chunkProvider.getChunks().begin(); iter != _chunkProvider.getChunks().end(); iter++)
		{
			ChunkPtr updateChunk = iter->second;
			int updateRange = 30;
			ChunkPos leftBound = ChunkPos(BlockPos(playerPos.x - updateRange, playerPos.y));
			bool outOfRangeLeft = updateChunk->getPos().x < leftBound.x;
			ChunkPos rightBound = ChunkPos(BlockPos(playerPos.x + updateRange, playerPos.y));
			bool outOfRangeRight = rightBound.x < updateChunk->getPos().x;
			// TODO: Vertical checking?
			if (!outOfRangeLeft && !outOfRangeRight)
			{
				updateChunk->tick();
			}
		}
	}
}