#pragma once

#include "Apollo/render/Texture.h"

#include "Apollo/World/World.h"
#include "Apollo/render/WorldRenderer.h"
#include "Apollo/render/2d/OrthographicCamera.h"

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

		BlockPos translateToPlanetPos(BlockPos pos);
	public:
		Planet(OrthographicCamera& renderCam, Texture::TextureArray blockTextures);
		~Planet();

		void render();

		Block getBlockAt(BlockPos blockPos);
		void setBlockAt(BlockPos blockPos, int blockID);
		void setBlockAt(BlockPos blockPos, Block block);
	};
}