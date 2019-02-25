#include "Player.h"

namespace Apollo
{
	Player::Player(Sprite sprite) : _sprite(sprite)
	{

	}

	Player::~Player()
	{

	}

	void Player::draw(Shader& shader)
	{
		shader.uniform("transformMatrix", _sprite.getTransformMatrix());
		_sprite.draw();
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