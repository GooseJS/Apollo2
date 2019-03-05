#define STB_IMAGE_IMPLEMENTATION

#include <Apollo/window/Window.h>
#include <Apollo/GameSettings.h>
#include <Apollo/logger/Logger.h>
#include <Apollo/render/Shader.h>
#include <Apollo/render/2d/Sprite.h>
#include <Apollo/render/2d/DebugRenderer.h>
#include <Apollo/render/2d/imgui/ApolloImGui.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Apollo/World/World.h"
#include "Apollo/render/WorldRenderer.h"

#include "Player.h"

Apollo::Texture::TextureArray getBlockTextures()
{
	std::vector<std::string> files;
	files.emplace_back("grass.png");
	files.emplace_back("dirt.png");
	return Apollo::Texture::create2DTextureArray(16, 16, files);
}

int main()
{
	{
		Apollo::Window window;

		if (window.createWindow() != APOLLO_NO_ERROR)
		{
			return -1;
		}

		Apollo::World world;
		Apollo::DebugRenderer debugRenderer;

		Apollo::Shader shader;
		shader.initFromFile("shader.vert", "shader.frag");
		Apollo::Sprite sprite;
		sprite.init(32.0f, 40.0f, 0, false);
		Apollo::Player player(world, Apollo::Rectangle(0.0f, 0.0f, 30.0f, 40.0f));
		player.getConfiguration().moveSpeed = 100.0f;
 
		Apollo::GameSettings::getInstance().setup();

		// Camera stuff
		Apollo::GameSettings::getInstance().windowCfg->scaleFactor = 2.0f;
		glm::mat4 cam = glm::ortho(0.0f, Apollo::GameSettings::getInstance().windowCfg->windowWidth / Apollo::GameSettings::getInstance().windowCfg->scaleFactor, 0.0f, Apollo::GameSettings::getInstance().windowCfg->windowHeight / Apollo::GameSettings::getInstance().windowCfg->scaleFactor);
		shader.uniform("cameraProjection", cam);

		debugRenderer.init(cam);

		Apollo::Texture::TextureArray blockTextures = getBlockTextures();

		Apollo::BlockManager::getInstance().addBlock("air", 0);
		Apollo::BlockManager::getInstance().addBlock("dirt", blockTextures.getEntry("dirt"));
		Apollo::BlockManager::getInstance().addBlock("grass", blockTextures.getEntry("grass"));

		Apollo::BlockManager::getInstance().getBlock(1).getData().hasCollision = true;
		Apollo::BlockManager::getInstance().getBlock(2).getData().hasCollision = true;

		world.setBlock(Apollo::BlockPos(15, 15), Apollo::BlockManager::getInstance().getBlock(2));

		for (int x = 0; x < 32; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				world.setBlock(Apollo::BlockPos(x, y), Apollo::BlockManager::getInstance().getBlock(y == 9 ? 2 : 1));
			}
		}

		Apollo::WorldRenderer worldRenderer(world, blockTextures);	

		player.setPos(glm::vec2(100, 500));

		// Setup Platform/Renderer bindings
		ImGuiContext* imGuiContext = Apollo::ApolloImGui::getInstance().init(window);
		ImGui::SetCurrentContext(imGuiContext);

		bool renderTestWindow = true;

		int currentlyPlacingBlock = 0;

		while (!window.shouldClose())
		{
			Apollo::GameSettings::getInstance().gameTime->newFrame(glfwGetTime());
			window.updateWindow();
			window.swapBuffers();
			glClearColor(0.3f, 0.9f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			Apollo::ApolloImGui::getInstance().newFrame();
			
			if (renderTestWindow)
				ImGui::ShowDemoWindow(&renderTestWindow);
			
			{ // Block selection window
				ImGui::Begin("Place Block"); // Creating a window called "Place Block"
			
				const char* blockNames[] = { "Dirt", "Grass" };
				ImGui::Combo("combo", &currentlyPlacingBlock, blockNames, IM_ARRAYSIZE(blockNames));
			
				ImGui::End();
			}

			Apollo::BlockManager::getInstance().drawBlockDebugData();
			player.drawDebugWindow();

			shader.use();
			player.draw(shader);
			player.debugDraw(debugRenderer);

			worldRenderer.checkForChunkUpdates();

			worldRenderer.draw();

			float mouseX = window.getMouseX() / Apollo::GameSettings::getInstance().windowCfg->scaleFactor;
			float mouseY = (Apollo::GameSettings::getInstance().windowCfg->windowHeight - window.getMouseY()) / Apollo::GameSettings::getInstance().windowCfg->scaleFactor;
			int mouseBlockX = floor(mouseX / 16.0f);
			int mouseBlockY = floor(mouseY / 16.0f);
			debugRenderer.addSquare(mouseBlockX * 16.0f, mouseBlockY  * 16.0f, 16, 16);
			
			if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
			{
				if (world.chunkExistsAt(Apollo::BlockPos(mouseBlockX, mouseBlockY)))
					if (world.getBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY)).blockID() != 0)
						world.setBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY), Apollo::BlockManager::getInstance().getBlock(0));
			}
			if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) && !ImGui::GetIO().WantCaptureMouse)
			{
				if (world.chunkExistsAt(Apollo::BlockPos(mouseBlockX, mouseBlockY)))
				{
					if (world.getBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY)).blockID() != currentlyPlacingBlock + 1)
						world.setBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY), Apollo::BlockManager::getInstance().getBlock(currentlyPlacingBlock + 1));
				}
				else
					world.setBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY), Apollo::BlockManager::getInstance().getBlock(currentlyPlacingBlock + 1));
			}

			debugRenderer.drawAndFlush();

			Apollo::MovementInput movementInput{};
			movementInput.upPressed = window.isKeyPressed(GLFW_KEY_W);
			movementInput.downPressed = window.isKeyPressed(GLFW_KEY_S);
			movementInput.leftPressed = window.isKeyPressed(GLFW_KEY_D);
			movementInput.rightPressed = window.isKeyPressed(GLFW_KEY_A);
			movementInput.jumpPressed = window.isKeyPressed(GLFW_KEY_SPACE);

			if (window.isKeyPressed(GLFW_KEY_Q))
				window.setShouldClose();

			player.move(movementInput);
			player.update();

			// ImGui Render
			Apollo::ApolloImGui::getInstance().render();
		}

		Apollo::GameSettings::getInstance().cleanup();
	}
}