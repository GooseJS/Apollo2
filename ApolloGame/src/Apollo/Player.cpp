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
		else if (_velocity.x < -_playerConfig.maxVelocity) _velocity.x = -_playerConfig.maxVelocity;
		if (_velocity.y > _playerConfig.maxVelocity) _velocity.y = _playerConfig.maxVelocity;
		else if (_velocity.y < -_playerConfig.maxVelocity) _velocity.y = -_playerConfig.maxVelocity; // TODO: Implement canFly
		_position += (_velocity * glm::vec2(Apollo::GameSettings::getInstance().gameTime->getDeltaTime()));

		_sprite.setPos(_position);
	}

	void Player::move(MovementInput input)
	{
		float frameMovementSpeed = _playerConfig.moveSpeed * Apollo::GameSettings::getInstance().gameTime->getDeltaTime();
		float frameJumpPower = _playerConfig.jumpPower * Apollo::GameSettings::getInstance().gameTime->getDeltaTime();
		if (input.leftPressed)
			_position += glm::vec2(frameMovementSpeed, 0.0f);
		if (input.rightPressed)
			_position -= glm::vec2(frameMovementSpeed, 0.0f);

		if (_capabilities.isJumping && !input.jumpPressed)
		{
			_velocity.y = 0.0f;
			_capabilities.isJumping = false;
		}

		if (input.jumpPressed)
		{
 			if (_capabilities.isJumping)
			{
				if (_velocity.y < -_playerConfig.jumpPower) // NOTE: If gravity starts winning the fight, we give up jumping
				{
					_velocity.y = 0.0f;
					_capabilities.isJumping = false;
				}
				else
				{
					_position += glm::vec2(0.0, frameJumpPower);
				}
			}
			else if (_internalData.onGround)
			{
				_capabilities.isJumping = true;
				_position += glm::vec2(0.0, frameJumpPower);
			}
		}
	}
}