#pragma once

#include <glm/glm.hpp>

#include <Apollo/GameSettings.h>
#include <Apollo/render/Shader.h>
#include <Apollo/render/2d/Sprite.h>
#include <Apollo/render/2d/DebugRenderer.h>

#include "Apollo/World/World.h"
#include "Apollo/render/Rectangle.h"

namespace Apollo
{
	struct MovementInput
	{
		bool upPressed = false;
		bool downPressed = false;
		bool leftPressed = false;
		bool rightPressed = false;

		bool jumpPressed = false;
	};

	struct PlayerCapabilities
	{
		bool canFly = true;
		bool isFlying = false;
	};

	struct PlayerConfiguration
	{
		float maxVelocity = 10.f;
		float gravity = 1.f;
		float moveSpeed = 5.f;
	};

	struct PlayerInternal
	{
		bool onGround;

		Rectangle floorCollisionRect;
	};

	class Player
	{
	private:
		glm::vec2 worldToBlockPos(glm::vec2 worldPos);

		World& _currentWorld;

		glm::vec2 _position = glm::vec2(1.0f);
		glm::vec2 _velocity = glm::vec2(0.0f);

		Sprite _sprite;

		Rectangle _playerBounds;
		
		PlayerCapabilities _capabilities;
		PlayerConfiguration _playerConfig;

		PlayerInternal _internalData;
	public:
		Player(World& world, Rectangle playerBounds);
		~Player();

		void draw(Shader& shader);
		void debugDraw(DebugRenderer& debugRenderer);

		void update();

		void move(MovementInput input);

		inline PlayerCapabilities& getCapabilities() { return _capabilities; }
		inline PlayerConfiguration& getConfiguration() { return _playerConfig; }

		inline void setPos(glm::vec2 pos) { _position = pos; }
		inline glm::vec2 getPos() { return _position; }
	};
}