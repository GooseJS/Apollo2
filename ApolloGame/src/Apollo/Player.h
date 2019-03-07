#pragma once

#include <glm/glm.hpp>

#include <Apollo/GameSettings.h>
#include <Apollo/render/Shader.h>
#include <Apollo/render/2d/Sprite.h>
#include <Apollo/render/2d/DebugRenderer.h>

#include "Apollo/World/World.h"
#include "Apollo/World/Planet.h"
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
		bool canFly = false;
		bool isFlying = false;

		bool collides = true;

		bool isJumping = false;
	};

	struct PlayerConfiguration
	{
		float maxVelocity = 200.f;

		float gravity = 1.f;

		float moveSpeed = 100.f;
		float flySpeed = 150.f;

		float jumpPower = 100.f;
	};

	struct PlayerInternal
	{
		bool onGround = false;
		bool collidingLeft = false;
		bool collidingRight = false;

		Rectangle leftCollisionRect;
		Rectangle rightCollisionRect;
		Rectangle floorCollisionRect;
	};

	class Player
	{
	private:
		glm::vec2 worldToBlockPos(glm::vec2 worldPos);

		Planet& _currentPlanet;

		glm::vec2 _position = glm::vec2(1.0f);
		glm::vec2 _velocity = glm::vec2(0.0f);

		Sprite _sprite;
		Shader& _shader;

		Rectangle _playerBounds;
		
		PlayerCapabilities _capabilities;
		PlayerConfiguration _playerConfig;

		PlayerInternal _internalData;

		void updateCollisionRects();

		void checkForCollisons();
	public:
		Player(Planet& planet, Rectangle playerBounds, Shader& shader);
		~Player();

		void draw();
		void debugDraw(DebugRenderer& debugRenderer);

		void update();

		void move(MovementInput input);

		void drawDebugWindow();

		inline PlayerCapabilities& getCapabilities() { return _capabilities; }
		inline PlayerConfiguration& getConfiguration() { return _playerConfig; }

		inline void setPos(glm::vec2 pos) { _position = pos; }
		inline glm::vec2 getPos() { return _position; }

		inline BlockPos getBlockPos() { return BlockPos(_position.x / APOLLO_BLOCK_WIDTH, _position.y / APOLLO_BLOCK_WIDTH); }
	};
}