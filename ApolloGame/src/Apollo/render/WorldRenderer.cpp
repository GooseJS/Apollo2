#include "Apollo/render/WorldRenderer.h"
#include <Apollo/logger/Logger.h>

namespace Apollo
{
	WorldRenderer::WorldRenderer(World& world, Texture::TextureArray textureArray, OrthographicCamera& camera) : _world(world), _blockTextures(textureArray), _camera(camera)
	{
		_worldShader.initFromFile("worldShader.vert", "worldShader.frag");
		_worldShader.use();
	}

	void WorldRenderer::initChunk(ChunkPos pos)
	{
		struct ChunkVertex
		{
			float x = 0;
			float y = 0;
			float tx = 0;
			float ty = 0;
			float tz = 0;

			ChunkVertex() {}
			ChunkVertex(float _x, float _y, float _tx, float _ty, float _tz) : x(_x), y(_y), tx(_tx), ty(_ty), tz(_tz) {}
		};

		ChunkPtr chunk = _world.getChunkAt(pos);

		if (chunk->getMesh().needsUpdate)
		{

			if (!chunk->getMesh().init)
			{
				glGenVertexArrays(1, &chunk->getMesh().vaoID);
			}

			glBindVertexArray(chunk->getMesh().vaoID);

			if (!chunk->getMesh().init)
			{
				glGenBuffers(1, &chunk->getMesh().vboID);
				glBindBuffer(GL_ARRAY_BUFFER, chunk->getMesh().vboID);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (GLvoid*)offsetof(ChunkVertex, x));
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (GLvoid*)offsetof(ChunkVertex, tx));

				chunk->getMesh().init = true;
			}

			glBindBuffer(GL_ARRAY_BUFFER, chunk->getMesh().vboID);

			std::vector<ChunkVertex> vertices;

			for (int x = 0; x < APOLLO_CHUNK_WIDTH; x++)
			{
				for (int y = 0; y < APOLLO_CHUNK_WIDTH; y++)
				{
					Block currentBlock = _world.getBlock(BlockPos(x + (pos.x * APOLLO_CHUNK_WIDTH), y + (pos.y * APOLLO_CHUNK_WIDTH)));
					if (currentBlock.blockID() > 0)
					{
						float translatedX = x * _blockSize;
						float translatedY = y * _blockSize;
						float translatedX1 = translatedX + _blockSize;
						float translatedY1 = translatedY + _blockSize;
						vertices.emplace_back(translatedX, translatedY, 0, 1, currentBlock.textureID());	// 0, 0 | 0, 1
						vertices.emplace_back(translatedX1, translatedY1, 1, 0, currentBlock.textureID());	// 1, 1 | 1, 0
						vertices.emplace_back(translatedX, translatedY1, 0, 0, currentBlock.textureID());	// 0, 1 | 0, 0

						vertices.emplace_back(translatedX1, translatedY1, 1, 0, currentBlock.textureID());	// 1, 1 | 1, 0
						vertices.emplace_back(translatedX, translatedY, 0, 1, currentBlock.textureID());	// 0, 0 | 0, 1
						vertices.emplace_back(translatedX1, translatedY, 1, 1, currentBlock.textureID());	// 1, 0 | 1, 1
					}
				}
			}

			chunk->getMesh().numVertices = vertices.size();

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ChunkVertex), &vertices[0].x, GL_STATIC_DRAW);

			chunk->getMesh().needsUpdate = false;
		}
	}

	void WorldRenderer::checkForChunkUpdates()
	{
		ChunkMap& loadedChunks = _world.getRenderChunks();
		for (auto chunkIter = loadedChunks.begin(); chunkIter != loadedChunks.end(); chunkIter++)
		{
			ChunkPtr chunk = chunkIter->second;
			if (chunk->getMesh().needsUpdate)
				initChunk(chunk->getPos());
		}
	}

	void WorldRenderer::draw()
	{
		glm::mat4 translation(1.0f);

		glBindTexture(GL_TEXTURE_2D_ARRAY, _blockTextures.textureID);
		glActiveTexture(GL_TEXTURE0);

		_camera.uploadMatrix(_worldShader, "orthoProjection");

		for (auto iter = _world.getRenderChunks().begin(); iter != _world.getRenderChunks().end(); iter++)
		{
			ChunkPtr chunk = iter->second;
			bool outOfBoundsLeft = (_camera.xOffset() > (chunk->getPos().x - _chunkRenderBuffer) * APOLLO_CHUNK_WIDTH * APOLLO_BLOCK_WIDTH);
			bool outOfBoundsRight = (_camera.xOffset() + _camera.width() < (chunk->getPos().x + _chunkRenderBuffer) * APOLLO_CHUNK_WIDTH * APOLLO_BLOCK_WIDTH);
			bool outOfBoundsBottom = false; // TODO (Brendan): Implement if needed?
  			bool outOfBoundsTop = false;

			if (outOfBoundsLeft || outOfBoundsRight || outOfBoundsBottom || outOfBoundsTop)
			{
				// TODO (Brendan): Unload / delete chunk and VAO data!
				continue;
			}

			glBindVertexArray(chunk->getMesh().vaoID);

			translation = glm::translate(translation, glm::vec3(chunk->getPos().x * APOLLO_BLOCK_WIDTH * APOLLO_CHUNK_WIDTH, chunk->getPos().y * APOLLO_BLOCK_WIDTH * APOLLO_CHUNK_WIDTH, 0.f));
			_worldShader.use();
			_worldShader.uniform("translation", translation);
			translation = glm::mat4(1.0f);

			glDrawArrays(GL_TRIANGLES, 0, chunk->getMesh().numVertices);
		}
	}
}