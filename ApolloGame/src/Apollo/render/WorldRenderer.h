#pragma once

#include <glm/glm.hpp>

#include <Apollo/render/Shader.h>
#include <Apollo/render/Texture.h>
#include <Apollo/GameSettings.h>

#include "Apollo/World/World.h"

namespace Apollo
{
	class WorldRenderer
	{
	private:
		World& _world;
		Shader _worldShader;

		Texture::TextureArray _blockTextures;

		float _blockSize = 16.0f;
	public:
		WorldRenderer(World& world, Texture::TextureArray blockTextures);

		void initChunk(ChunkPos pos);

		void draw();

		void checkForChunkUpdates();
	};
}