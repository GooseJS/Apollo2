#pragma once

#include <vector>

#include "Apollo/World/Block.h"
#include "Apollo/World/WorldTypes.h"

#include "Apollo/render/ChunkMesh.h"

namespace Apollo
{
	class Chunk
	{
	private:
		ChunkPos _pos;
		int _blocks[APOLLO_CHUNK_AREA]{};
		ChunkMesh _mesh{};
	public:
		Chunk(ChunkPos pos);
		void setBlock(LocalBlockPos pos, Block block);
		Block getBlock(LocalBlockPos pos);

		inline ChunkMesh& getMesh() { return _mesh; }
		inline ChunkPos getPos() { return _pos; }
	};
}