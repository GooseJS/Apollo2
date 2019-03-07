#pragma once

#include "Apollo/render/Texture.h"

#include "Apollo/World/World.h"
#include "Apollo/render/WorldRenderer.h"
#include "Apollo/render/2d/OrthographicCamera.h"
#include "Apollo/World/tileEntities/TileEntity.h"

namespace Apollo
{
	class Planet
	{
	private:
		World _world;
		WorldRenderer _worldRenderer;

		OrthographicCamera& _renderCam;

		int _planetSize;

		std::unordered_map<ChunkPtr, glm::vec2> _renderingChunks;
	public:
		Planet(int planetSize, OrthographicCamera& renderCam, Texture::TextureArray blockTextures);
		~Planet();

		BlockPos translateToPlanetPos(BlockPos pos);
		ChunkPos translateChunkToPlanetPos(ChunkPos pos);

		void render();

		Block getBlockAt(BlockPos blockPos);
		TileEntityPtr getTileEntityAt(BlockPos pos);
		void setBlockAt(BlockPos blockPos, int blockID, TileEntityPtr tileEntity = nullptr);
		void setBlockAt(BlockPos blockPos, Block block, TileEntityPtr tileEntity = nullptr);

		void tick(BlockPos playerPos);

		inline World& getWorld() { return _world; }
		inline int getSize() { return _planetSize; }
	};
}