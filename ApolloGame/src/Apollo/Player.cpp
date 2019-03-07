#include "Player.h"

namespace Apollo
{
	Player::Player(World& world, Rectangle playerBounds, Shader& shader) : _currentWorld(world), _playerBounds(playerBounds), _shader(shader)
	{
		_sprite.init(playerBounds.getWidth(), playerBounds.getHeight(), false);

		_internalData.leftCollisionRect.setWidth(4.0f);
		_internalData.leftCollisionRect.setHeight(32.0f);

		_internalData.rightCollisionRect.setWidth(4.0f);
		_internalData.rightCollisionRect.setHeight(32.0f);

		_internalData.floorCollisionRect.setWidth(20.0f);
		_internalData.floorCollisionRect.setHeight(4.0f);
	}

	Player::~Player()
	{

	}

	void Player::updateCollisionRects()
	{
		// TODO: Abstract these values into some variable
		_internalData.leftCollisionRect.setPos(_position.x - 0.2f, _position.y + 4.0f);
		_internalData.rightCollisionRect.setPos(_position.x + 30.0f - 4.0f + 0.2f, _position.y + 4.0f);
		_internalData.floorCollisionRect.setPos(_position.x + 5.0f, _position.y - 0.2f);
	}

	void Player::checkForCollisons()
	{
		_internalData.onGround = false;
		std::vector<BlockPos> floorCollisionBlocks = _internalData.floorCollisionRect.getContainingBlockPos();
		for (auto checkingFloorCollisionBlock : floorCollisionBlocks)
		{
			if (_currentWorld.chunkExistsAt(checkingFloorCollisionBlock))
			{
				Block currentBlock = _currentWorld.getBlock(checkingFloorCollisionBlock);
				if (currentBlock.getData().hasCollision)
				{
					_internalData.onGround = true;
					break;
				}
			}
		}

		_internalData.collidingLeft = false;
		std::vector<BlockPos> leftCollisionBlocks = _internalData.leftCollisionRect.getContainingBlockPos();
		for (auto leftCollisionBlock : leftCollisionBlocks)
		{
			if (_currentWorld.chunkExistsAt(leftCollisionBlock))
			{
				Block currentBlock = _currentWorld.getBlock(leftCollisionBlock);
				if (currentBlock.getData().hasCollision)
				{
					_internalData.collidingLeft = true;
					break;
				}
			}
		}

		_internalData.collidingRight = false;
		std::vector<BlockPos> rightCollisionBlocks = _internalData.rightCollisionRect.getContainingBlockPos();
		for (auto rightCollisionBlock : rightCollisionBlocks)
		{
			if (_currentWorld.chunkExistsAt(rightCollisionBlock))
			{
				Block currentBlock = _currentWorld.getBlock(rightCollisionBlock);
				if (currentBlock.getData().hasCollision)
				{
					_internalData.collidingRight = true;
					break;
				}
			}
		}
	}

	glm::vec2 Player::worldToBlockPos(glm::vec2 worldPos)
	{
		return worldPos / 16.f;
	}

	void Player::draw()
	{
		_shader.use();
		_shader.uniform("transformMatrix", _sprite.getTransformMatrix());
		_sprite.draw();
		//BlockPos localBlockPos = 
	}

	void Player::debugDraw(DebugRenderer& debugRenderer)
	{
		float collision = _internalData.collidingLeft ? 1.0f : 0.0f;
		debugRenderer.addSquare(_internalData.leftCollisionRect, 0.0f, collision, 0.75f, 0.5f);
		debugRenderer.addSquare(_internalData.leftCollisionRect, 0.0f, 0.0f, 1.0f, 1.0f, true);

		collision = _internalData.collidingRight ? 1.0f : 0.0f;
		debugRenderer.addSquare(_internalData.rightCollisionRect, 0.0f, collision, 0.75f, 0.5f);
		debugRenderer.addSquare(_internalData.rightCollisionRect, 0.0f, 0.0f, 1.0f, 1.0f, true);

		float r = _internalData.onGround ? 1.0f : 0.0f;
		debugRenderer.addSquare(_internalData.floorCollisionRect, 0.0f, r, 0.75f, 0.5f);
		debugRenderer.addSquare(_internalData.floorCollisionRect, 0.0f, 0.0f, 1.0f, 1.0f, true);
	}

	void Player::update()
	{
		updateCollisionRects();

		checkForCollisons();

		if (_capabilities.collides && _internalData.onGround)
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
		else if (_velocity.y < -_playerConfig.maxVelocity) _velocity.y = -_playerConfig.maxVelocity;
		if (_capabilities.isFlying) { _velocity.x = 0; _velocity.y = 0; }
		_position += (_velocity * glm::vec2(Apollo::GameSettings::getInstance().gameTime->getDeltaTime(), Apollo::GameSettings::getInstance().gameTime->getDeltaTime()));

		_sprite.setPos(_position);
	}

	void Player::move(MovementInput input)
	{
		float frameMovementSpeed = (_capabilities.isFlying ? _playerConfig.flySpeed : _playerConfig.moveSpeed) * Apollo::GameSettings::getInstance().gameTime->getDeltaTime();
		float frameJumpPower = _playerConfig.jumpPower * Apollo::GameSettings::getInstance().gameTime->getDeltaTime();

		if (!_capabilities.isFlying)
		{
			if (input.leftPressed && (!_capabilities.collides || !_internalData.collidingRight)) // I'm so confused????
				_position += glm::vec2(frameMovementSpeed, 0.0f);
			if (input.rightPressed && (!_capabilities.collides || !_internalData.collidingLeft))
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
				else if (_capabilities.collides && _internalData.onGround)
				{
					_capabilities.isJumping = true;
					_position += glm::vec2(0.0, frameJumpPower);
				}
			}
		}
		else
		{
			if (input.leftPressed && (!_capabilities.collides || !_internalData.collidingRight)) // Once again, still so fucking confused
				_position += glm::vec2(frameMovementSpeed, 0.0f);
			if (input.rightPressed && (!_capabilities.collides || !_internalData.collidingLeft))
				_position -= glm::vec2(frameMovementSpeed, 0.0f);
			if (input.upPressed)
				_position += glm::vec2(0.0f, frameMovementSpeed);
			if (input.downPressed && (!_capabilities.collides || !_internalData.onGround))
				_position -= glm::vec2(0.0f, frameMovementSpeed);
		}
	}

	void Player::drawDebugWindow()
	{
		ImGui::Begin("Player", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		BlockPos blockPos = getBlockPos();
		ChunkPos chunkPos(blockPos);
		ImGui::Text("Current Position: %i, %i", blockPos.x, blockPos.y);
		ImGui::Text("Chunk Postion: %i, %i", chunkPos.x, chunkPos.y);

		ImGui::Checkbox("Collides", &_capabilities.collides);
		ImGui::Checkbox("Is Flying", &_capabilities.isFlying);
		ImGui::InputFloat("Jump Power", &_playerConfig.jumpPower, 5.0f, 10.0f, "%.2f");
		ImGui::Text(" -- Player Speed -- ");
		ImGui::Text(" - Move Speed -"); ImGui::SameLine(); ImGui::InputFloat("", &_playerConfig.moveSpeed, 0.5f, 1.0f, "%.1f");
		ImGui::Text(" - Fly Speed -"); ImGui::SameLine(); ImGui::InputFloat("", &_playerConfig.flySpeed, 0.5f, 1.0f, "%.1f");

		ImGui::End();
	}
}