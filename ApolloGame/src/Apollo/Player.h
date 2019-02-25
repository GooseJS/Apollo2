#pragma once

#include <glm/glm.hpp>

#include <Apollo/GameSettings.h>
#include <Apollo/render/2d/Sprite.h>

namespace Apollo
{
	struct MovementInput
	{
		bool upPressed = false;
		bool downPressed = false;
		bool leftPressed = false;
		bool rightPressed = false;
	};

	struct PlayerCapabilities
	{
		bool canFly = true;
		bool isFlying = false;
	};

	struct PlayerConfiguration
	{
		float moveSpeed = 5.f;
	};

	class Player
	{
	private:
		glm::vec2 _position = glm::vec2(1.0f);
		Apollo::Sprite _sprite;
		
		PlayerCapabilities _capabilities;
		PlayerConfiguration _playerConfig;
	public:
		Player(Sprite sprite);
		~Player();

		void draw();

		void move(MovementInput input);
	};
}