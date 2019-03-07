#pragma once

#include <glm/glm.hpp>

#include <Apollo/GameSettings.h>
#include <Apollo/render/Shader.h>
#include <Apollo/render/Texture.h>
#include <Apollo/render/2d/OrthographicCamera.h>

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

		OrthographicCamera& _camera;
	public:
		WorldRenderer(World& world, Texture::TextureArray blockTextures, OrthographicCamera& camera);

		void initChunk(ChunkPos pos);

		void draw();

		void checkForChunkUpdates();
	};
}