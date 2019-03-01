#include "Player.h"

namespace Apollo
{
	Player::Player(World world, Sprite sprite) : _currentWorld(world), _sprite(sprite)
	{

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

	void Player::debugDraw(Shader& shader)
	{

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