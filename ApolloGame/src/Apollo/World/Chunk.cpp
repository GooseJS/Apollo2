#include "Apollo/World/Chunk.h"

namespace Apollo
{
	Chunk::Chunk(ChunkPos pos) : _pos(pos)
	{

	}

	void Chunk::setBlock(LocalBlockPos pos, Block block, TileEntityPtr tileEntity)
	{
		_blocks[pos.index()] = block.blockID();
		_mesh.needsUpdate = true;
		if (tileEntity != nullptr)
			_tileEntities.insert(std::make_pair(pos.index(), std::move(tileEntity)));
	}

	Block Chunk::getBlock(LocalBlockPos pos)
	{
		Block block = BlockManager::getInstance().getBlock(_blocks[pos.index()]);
		return block;
	}

	TileEntityPtr Chunk::getTileEntity(LocalBlockPos pos)
	{
		TileEntityPtr tileEntity = _tileEntities.at(pos.index());
		return tileEntity;
	}

	void Chunk::cleanTileEntity(LocalBlockPos pos)
	{
		TileEntityPtr tileEntity = _tileEntities.at(pos.index());
		_tileEntities.erase(pos.index());
		delete tileEntity;
	}

	void Chunk::tick()
	{
		for (auto iter = _tileEntities.begin(); iter != _tileEntities.end(); iter++)
		{
			iter->second->tick();
		}
	}
}