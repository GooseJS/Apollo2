#include "Apollo/World/Planet.h"

namespace Apollo
{
	Planet::Planet(int planetSize, OrthographicCamera& cam, Texture::TextureArray blockTextures) : _planetSize(planetSize), _renderCam(cam), _worldRenderer(_world, blockTextures, cam)
	{

	}

	Planet::~Planet()
	{

	}

	BlockPos Planet::translateToPlanetPos(BlockPos pos)
	{
		int x = pos.x;
		if (pos.x >= _planetSize)
			x -= _planetSize;
		if (pos.x < 0)
			x += _planetSize;
		return BlockPos(x, pos.y);
	}

	ChunkPos Planet::translateChunkToPlanetPos(ChunkPos pos)
	{
		int x = pos.x;
		if (pos.x < 0)
			x += _planetSize / APOLLO_CHUNK_WIDTH;
		if (pos.x > _planetSize / APOLLO_CHUNK_WIDTH)
			x -= _planetSize / APOLLO_CHUNK_WIDTH;
		return ChunkPos(x, pos.y);
	}

	void Planet::render()
	{
		_worldRenderer.checkForChunkUpdates();

		glm::vec3 additionalTranslation(0.0f);

		_worldRenderer.draw();
		if (_renderCam.xOffset() < 0) // TODO: Scaling factor have to be taken into account here?
		{
			// Have to render other side of planet (from 0 -> planetSize)
			float cachedRenderCamX = _renderCam.xOffset();
			float cachedRenderCamY = _renderCam.yOffset();

			_renderCam.setPos(_renderCam.xOffset() + (_planetSize * APOLLO_BLOCK_WIDTH), _renderCam.yOffset());
			_worldRenderer.draw();
			_renderCam.setPos(cachedRenderCamX, cachedRenderCamY);
		}
		else if (_renderCam.xOffset() + _renderCam.width() > (_planetSize * APOLLO_BLOCK_WIDTH))
		{
			// Have to render other side of planet (from planetSize -> 0)
			float cachedRenderCamX = _renderCam.xOffset();
			float cachedRenderCamY = _renderCam.yOffset();

			_renderCam.setPos(_renderCam.xOffset() - (_planetSize * APOLLO_BLOCK_WIDTH), _renderCam.yOffset());
			_worldRenderer.draw();
			_renderCam.setPos(cachedRenderCamX, cachedRenderCamY);
		}
	}

	Block Planet::getBlockAt(BlockPos pos)
	{
		return _world.getBlock(translateToPlanetPos(pos));
	}

	TileEntityPtr Planet::getTileEntityAt(BlockPos pos)
	{
		return _world.getTileEntity(translateToPlanetPos(pos));
	}

	void Planet::setBlockAt(BlockPos pos, int blockID, TileEntityPtr tileEntity)
	{
		setBlockAt(pos, Apollo::BlockManager::getInstance().getBlock(blockID), tileEntity);
	}

	void Planet::setBlockAt(BlockPos pos, Block block, TileEntityPtr tileEntity)
	{
		_world.setBlock(translateToPlanetPos(pos), block, tileEntity);
	}

	void Planet::tick(BlockPos playerPos)
	{
		int updateRange = 30;
		ChunkPos initialLeftBound = ChunkPos(BlockPos(playerPos.x - updateRange, playerPos.y));
		ChunkPos leftBound = translateChunkToPlanetPos(initialLeftBound);
		bool outOfRangeLeft;

		ChunkPos initialRightBound = ChunkPos(BlockPos(playerPos.x + updateRange, playerPos.y));
		ChunkPos rightBound = translateChunkToPlanetPos(initialRightBound);
		bool outOfRangeRight;

		for (auto iter = _world.getChunks().begin(); iter != _world.getChunks().end(); iter++)
		{
			ChunkPtr updateChunk = iter->second;

			if (initialLeftBound.x != leftBound.x)
				outOfRangeLeft = (updateChunk->getPos().x + _planetSize / APOLLO_CHUNK_WIDTH) < leftBound.x;
			else
				outOfRangeLeft = updateChunk->getPos().x < leftBound.x;

			if (initialRightBound.x != rightBound.x)
				outOfRangeRight = rightBound.x < (updateChunk->getPos().x - _planetSize / APOLLO_CHUNK_WIDTH);
			else
				outOfRangeRight = rightBound.x > updateChunk->getPos().x;
			// TODO: Vertical checking?
			if (!outOfRangeLeft || !outOfRangeRight)
			{
				updateChunk->tick();
			}
		}
	}
}