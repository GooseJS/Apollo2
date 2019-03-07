#define STB_IMAGE_IMPLEMENTATION

#include <glm/gtc/type_ptr.hpp>
#include <Apollo/GameSettings.h>
#include <Apollo/window/Window.h>
#include <Apollo/logger/Logger.h>
#include <Apollo/render/Shader.h>
#include <Apollo/render/2d/Sprite.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Apollo/render/2d/DebugRenderer.h>
#include <Apollo/render/2d/imgui/ApolloImGui.h> 
#include <Apollo/render/2d/OrthographicCamera.h>

#include "Apollo/World/World.h"
#include "Apollo/World/Chunk.h"
#include "Apollo/render/WorldRenderer.h"

#include "Apollo/World/tileEntities/TileEntityChest.h"

#include "Apollo/World/generation/EarthWorldGenerator.h"

#include "Player.h"

namespace Apollo
{
	class ApolloGame
	{
	private:
		Window _window;
		
		Player _player;
		World _world;
	};
}

Apollo::Texture::TextureArray getBlockTextures()
{
	std::vector<std::string> files;
	files.emplace_back("grass.png");
	files.emplace_back("dirt.png");
	files.emplace_back("chest.png");
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

		Apollo::Shader shader;
		shader.initFromFile("shader.vert", "shader.frag");
		Apollo::Player player(world, Apollo::Rectangle(0.0f, 0.0f, 30.0f, 40.0f), shader);
 
		Apollo::GameSettings::getInstance().setup();

		// Camera stuff
		Apollo::GameSettings::getInstance().windowCfg->scaleFactor = 2.0f;
		Apollo::OrthographicCamera cam(0.0f, 0.0f, Apollo::GameSettings::getInstance().windowCfg->windowWidth / Apollo::GameSettings::getInstance().windowCfg->scaleFactor, Apollo::GameSettings::getInstance().windowCfg->windowHeight / Apollo::GameSettings::getInstance().windowCfg->scaleFactor, true);
		cam.uploadMatrix(shader, "cameraProjection");

		Apollo::DebugRenderer debugRenderer(cam);

		debugRenderer.init();

		Apollo::Texture::TextureArray blockTextures = getBlockTextures();

		Apollo::BlockManager::getInstance().addBlock("air", 0);
		Apollo::BlockManager::getInstance().addBlock("dirt", blockTextures.getEntry("dirt"));
		Apollo::BlockManager::getInstance().addBlock("grass", blockTextures.getEntry("grass"));
		Apollo::BlockManager::getInstance().addBlock("chest", blockTextures.getEntry("chest"));

		Apollo::EarthWorldGenerator worldGenerator(50);

		world.setBlock(Apollo::BlockPos(15, 15), Apollo::BlockManager::getInstance().getBlock(2));

		for (int x = 0; x < 500; x++)
		{
			int y = worldGenerator.getTopBlockHeightAt(x);
			if (y > 0)
			{
				world.setBlock(Apollo::BlockPos(x, y), Apollo::BlockManager::getInstance().getBlock(2));
				for (int placingY = y - 1; placingY > 0; placingY--)
				{
					world.setBlock(Apollo::BlockPos(x, placingY), Apollo::BlockManager::getInstance().getBlock(1));
				}
			}
		}

		Apollo::WorldRenderer worldRenderer(world, blockTextures, cam);	

		player.setPos(glm::vec2(100, 500));

		// Setup Platform/Renderer bindings
		ImGuiContext* imGuiContext = Apollo::ApolloImGui::getInstance().init(window);
		ImGui::SetCurrentContext(imGuiContext);

		bool renderTestWindow = true;

		int currentlyPlacingBlock = 0;

		int oof = 0;

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
			
				const char* blockNames[] = { "Dirt", "Grass", "Chest" };
				ImGui::Combo("combo", &currentlyPlacingBlock, blockNames, IM_ARRAYSIZE(blockNames));
			
				ImGui::End();
			}

			Apollo::BlockManager::getInstance().drawBlockDebugData();
			player.drawDebugWindow();

			shader.use();
			cam.uploadMatrix(shader, "cameraProjection");
			player.draw();
			player.debugDraw(debugRenderer);

			worldRenderer.checkForChunkUpdates();

			worldRenderer.draw();

			float mouseX = (window.getMouseX() / Apollo::GameSettings::getInstance().windowCfg->scaleFactor) + cam.xOffset();
			float mouseY = ((Apollo::GameSettings::getInstance().windowCfg->windowHeight - window.getMouseY()) / Apollo::GameSettings::getInstance().windowCfg->scaleFactor) + cam.yOffset();
			int mouseBlockX = floor(mouseX / 16.0f);
			int mouseBlockY = floor(mouseY / 16.0f);
			debugRenderer.addSquare(mouseBlockX * 16.0f, mouseBlockY  * 16.0f, 16, 16);
			
			if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
			{
				if (mouseBlockX >= 0 && mouseBlockY >= 0)
				{
					if (world.chunkExistsAt(Apollo::BlockPos(mouseBlockX, mouseBlockY)))
					{
						Apollo::Block block = world.getBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY));
						if (block.blockID() == 3)
						{
							AP_INFO("{}", static_cast<Apollo::TileEntityChest*>(world.getTileEntity(Apollo::BlockPos(mouseBlockX, mouseBlockY)))->getStoredValue());
						}
						if (block.blockID() != 0)
							world.setBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY), Apollo::BlockManager::getInstance().getBlock(0));
					}
				}
			}
			if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) && !ImGui::GetIO().WantCaptureMouse)
			{
				if (mouseBlockX >= 0 && mouseBlockY >= 0)
				{
					if (world.chunkExistsAt(Apollo::BlockPos(mouseBlockX, mouseBlockY)))
					{
						if (world.getBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY)).blockID() != currentlyPlacingBlock + 1)
						{
							Apollo::TileEntityChest* tileEntity = nullptr;
							if (currentlyPlacingBlock == 2)
							{
								tileEntity = new Apollo::TileEntityChest(Apollo::BlockPos(mouseBlockX, mouseBlockY));
								tileEntity->setStoredValue(oof);
								oof++;
							}
							world.setBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY), Apollo::BlockManager::getInstance().getBlock(currentlyPlacingBlock + 1), tileEntity);
						}
					}
					else
					{
						Apollo::TileEntityChest* tileEntity = nullptr;
						if (currentlyPlacingBlock == 2)
						{
							tileEntity = new Apollo::TileEntityChest(Apollo::BlockPos(mouseBlockX, mouseBlockY));
							tileEntity->setStoredValue(oof);
							oof++;
						}
						world.setBlock(Apollo::BlockPos(mouseBlockX, mouseBlockY), Apollo::BlockManager::getInstance().getBlock(currentlyPlacingBlock + 1), tileEntity);
					}
				}
			}

			debugRenderer.drawAndFlush();

			world.tick(player.getBlockPos());

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

			cam.setPos(player.getPos().x - 16 - (cam.width() / 2.f), player.getPos().y - 16 - (cam.height() / 2.0f));

			// ImGui Render
			Apollo::ApolloImGui::getInstance().render();
		}

		Apollo::GameSettings::getInstance().cleanup();
	}
}