#include "Apollo/World/Chunk.h"

namespace Apollo
{
	Chunk::Chunk(ChunkPos pos) : _pos(pos)
	{

	}

	void Chunk::setBlock(LocalBlockPos pos, Block block)
	{
		_blocks[pos.index()] = block.blockID();
	}

	Block Chunk::getBlock(LocalBlockPos pos)
	{
		return BlockManager::getInstance().getBlock(_blocks[pos.index()]);
	}
}