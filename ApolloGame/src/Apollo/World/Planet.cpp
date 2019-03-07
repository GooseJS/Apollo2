//#include "Apollo/World/Planet.h"
//
//namespace Apollo
//{
//	Planet::Planet(OrthographicCamera& cam, Texture::TextureArray blockTextures) : _renderCam(cam), _worldRenderer(_world, blockTextures, cam)
//	{
//
//	}
//
//	Planet::~Planet()
//	{
//
//	}
//
//	BlockPos Planet::translateToPlanetPos(BlockPos pos)
//	{
//		int x = pos.x;
//		if (pos.x > _planetSize)
//			x -= _planetSize;
//		if (pos.x < 0)
//			x += _planetSize;
//		return BlockPos(x, pos.y);
//	}
//
//	void Planet::render()
//	{
//		_worldRenderer.checkForChunkUpdates();
//
//		glm::vec3 additionalTranslation(0.0f);
//
//		if (_renderCam.xOffset < 0) // TODO: Scaling factor have to be taken into account here?
//		{
//			// Have to render other side of planet (from 0 -> planetSize)
//			int negativeChunksNeeded = ceil(abs(_renderCam.xOffset / (float)APOLLO_BLOCK_WIDTH / (float)APOLLO_CHUNK_WIDTH)) + _worldRenderer.getChunkRenderBuffer();
//			for (int i = 0; i < negativeChunksNeeded; i++)
//			{
//
//			}
//		}
//		else if (_renderCam.xOffset > (_planetSize * APOLLO_BLOCK_WIDTH))
//		{
//			// Have to render other side of planet (from planetSize -> 0)
//		}
//		else
//		{
//			_worldRenderer.draw();
//			return;
//		}
//
//		glm::mat4 translation(1.0f);
//
//		glBindTexture(GL_TEXTURE_2D_ARRAY, _blockTextures.textureID);
//		glActiveTexture(GL_TEXTURE0);
//
//		_camera.uploadMatrix(_worldShader, "orthoProjection");
//
//		for (auto iter = _world.getRenderChunks().begin(); iter != _world.getRenderChunks().end(); iter++)
//		{
//			ChunkPtr chunk = iter->second;
//			bool outOfBoundsLeft = (_camera.xOffset() > (chunk->getPos().x - _chunkRenderBuffer) * APOLLO_CHUNK_WIDTH * APOLLO_BLOCK_WIDTH);
//			bool outOfBoundsRight = (_camera.xOffset() + _camera.width() < (chunk->getPos().x + _chunkRenderBuffer) * APOLLO_CHUNK_WIDTH * APOLLO_BLOCK_WIDTH);
//			bool outOfBoundsBottom = false; // TODO (Brendan): Implement if needed?
//			bool outOfBoundsTop = false;
//
//			if (outOfBoundsLeft || outOfBoundsRight || outOfBoundsBottom || outOfBoundsTop)
//			{
//				// TODO (Brendan): Unload / delete chunk and VAO data!
//				continue;
//			}
//
//			glBindVertexArray(chunk->getMesh().vaoID);
//
//			translation = glm::translate(translation, glm::vec3(chunk->getPos().x * APOLLO_BLOCK_WIDTH * APOLLO_CHUNK_WIDTH, chunk->getPos().y * APOLLO_BLOCK_WIDTH * APOLLO_CHUNK_WIDTH, 0.f));
//			_worldShader.use();
//			_worldShader.uniform("translation", translation);
//			translation = glm::mat4(1.0f);
//
//			glDrawArrays(GL_TRIANGLES, 0, chunk->getMesh().numVertices);
//		}
//	}
//
//	Block Planet::getBlockAt(BlockPos pos)
//	{
//		return _world.getBlock(translateToPlanetPos(pos));
//	}
//
//	void Planet::setBlockAt(BlockPos pos, int blockID)
//	{
//		_world.setBlock(translateToPlanetPos(pos), Apollo::BlockManager::getInstance().getBlock(blockID));
//	}
//
//	void Planet::setBlockAt(BlockPos pos, Block block)
//	{
//		_world.setBlock(translateToPlanetPos(pos), block);
//	}
//}