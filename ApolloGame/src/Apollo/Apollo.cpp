#include <Apollo/Window.h>
#include <Apollo/GameSettings.h>
#include <Apollo/logger/Logger.h>
#include <Apollo/render/Shader.h>
#include <Apollo/render/2d/Sprite.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Player.h"

int main()
{
	{
		Apollo::Window window;

		if (window.createWindow() != APOLLO_NO_ERROR)
		{
			return -1;
		}

		Apollo::Shader shader;
		shader.initFromFile("shader.vert", "shader.frag");
		Apollo::Sprite sprite;
		sprite.init(500.0f, 500.0f, 0, false);
		Apollo::Player player(sprite);

		Apollo::GameSettings::getInstance().setup();

		// Camera stuff
		glm::mat4 cam = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
		shader.uniform("cameraProjection", cam);

		while (!window.shouldClose())
		{
			Apollo::GameSettings::getInstance().gameTime->newFrame(glfwGetTime());
			window.updateWindow();
			window.swapBuffers();
			glClearColor(0.3f, 0.9f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shader.use();
			player.draw();

			Apollo::MovementInput movementInput{};
			movementInput.upPressed = window.isKeyPressed(GLFW_KEY_W);
			movementInput.downPressed = window.isKeyPressed(GLFW_KEY_S);
			movementInput.leftPressed = window.isKeyPressed(GLFW_KEY_D);
			movementInput.rightPressed = window.isKeyPressed(GLFW_KEY_A);

			player.move(movementInput);

			shader.uniform("transformMatrix", sprite.getTransformMatrix());
		}

		Apollo::GameSettings::getInstance().cleanup();
	}
}