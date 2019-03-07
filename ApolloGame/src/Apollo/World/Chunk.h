#pragma once

#include <map>
#include <vector>

#include "Apollo/World/Block.h"
#include "Apollo/World/WorldTypes.h"
#include "Apollo/World/blocks/BlockTickable.h"
#include "Apollo/World/tileEntities/TileEntity.h"

#include "Apollo/render/ChunkMesh.h"

namespace Apollo
{
	typedef TileEntity* TileEntityPtr;

	class Chunk
	{
	private:
		const ChunkPos _pos;

		int _blocks[APOLLO_CHUNK_AREA]{};
		std::unordered_map<int, TileEntityPtr> _tileEntities;

		ChunkMesh _mesh{};
	public:
		Chunk(ChunkPos pos);
		void setBlock(LocalBlockPos pos, Block block, TileEntityPtr tileEntity = nullptr);
		Block getBlock(LocalBlockPos pos);
		TileEntityPtr getTileEntity(LocalBlockPos pos);
		void cleanTileEntity(LocalBlockPos pos);

		void tick();

		inline ChunkMesh& getMesh() { return _mesh; }
		inline ChunkPos getPos() { return _pos; }
	};
}