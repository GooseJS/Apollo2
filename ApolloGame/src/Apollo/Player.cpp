#include "Player.h"

namespace Apollo
{
	Player::Player(World& world, Rectangle playerBounds) : _currentWorld(world), _playerBounds(playerBounds)
	{
		_sprite.init(playerBounds.getWidth(), playerBounds.getHeight(), false);
		_internalData.floorCollisionRect.setPos(_position.x + 5.0f, _position.y - 0.2f);
		_internalData.floorCollisionRect.setWidth(20.0f);
		_internalData.floorCollisionRect.setHeight(4.0f);
	}

	Player::~Player()
	{

	}

	glm::vec2 Player::worldToBlockPos(glm::vec2 worldPos)
	{
		return worldPos / 16.f;
	}

	void Player::draw(Shader& shader)
	{
		shader.uniform("transformMatrix", _sprite.getTransformMatrix());
		_sprite.draw();
		//BlockPos localBlockPos = 
	}

	void Player::debugDraw(DebugRenderer& debugRenderer)
	{
		float r = _internalData.onGround ? 1.0f : 0.0f;
		debugRenderer.addSquare(_internalData.floorCollisionRect, 0.0f, r, 0.75f, 0.5f);
		debugRenderer.addSquare(_internalData.floorCollisionRect, 0.0f, 0.0f, 1.0f, 1.0f, true);
	}

	void Player::update()
	{
		_internalData.floorCollisionRect.setPos(_position.x + 5.0f, _position.y - 0.2f);
		std::vector<BlockPos> floorCollisionBlocks = _internalData.floorCollisionRect.getContainingBlockPos();

		_internalData.onGround = false;
		for (auto checkingFloorCollisionBlock : floorCollisionBlocks)
		{
			Block currentBlock = _currentWorld.getBlock(checkingFloorCollisionBlock);
			if (currentBlock.getData().hasCollision)
			{
				_internalData.onGround = true;
				break;
			}
		}

		if (_internalData.onGround)
		{
			// NOTE: On ground!
			_velocity.y = 0.0f;
		}
		else
		{
			// NOTE: In air!
			_velocity.y -= _playerConfig.gravity;
		}

		if (_velocity.x > _playerConfig.maxVelocity) _velocity.x = _playerConfig.maxVelocity;
		if (_velocity.y > _playerConfig.maxVelocity) _velocity.y = _playerConfig.maxVelocity;
		_position += (_velocity * glm::vec2(Apollo::GameSettings::getInstance().gameTime->getDeltaTime()));
	}

	void Player::move(MovementInput input)
	{
		float frameMovementSpeed = _playerConfig.moveSpeed * Apollo::GameSettings::getInstance().gameTime->getDeltaTime();
		if (input.upPressed)
			_position += glm::vec2(0.0f, frameMovementSpeed); // TODO: MAKE AN EFFICIENT WAY TO ADD X / Y TO A GLM VEC2 IN ONE LINE
		if (input.downPressed)
			_position -= glm::vec2(0.0f, frameMovementSpeed);
		if (input.leftPressed)
			_position += glm::vec2(frameMovementSpeed, 0.0f);
		if (input.rightPressed)
			_position -= glm::vec2(frameMovementSpeed, 0.0f);

		_sprite.setPos(_position);
	}
}