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
#include "Apollo/World/Planet.h"

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
	files.emplace_back("textures/grass.png");
	files.emplace_back("textures/dirt.png"); // TODO (Brendan): Global Application ResourceManager class!!!! This is like really high on the list of shit to do
	files.emplace_back("textures/chest.png");
	return Apollo::Texture::create2DTextureArray(16, 16, files);
}

int main()
{
	{
		Apollo::ResourceManager::getInstance().setDataDir("GameData/");
		Apollo::Window window;

		if (window.createWindow() != APOLLO_NO_ERROR)
		{
			return -1;
		}

		//Apollo::World world;

		Apollo::Shader shader;
		shader.initFromFile("shaders/shader.vert", "shaders/shader.frag");
 
		Apollo::GameSettings::getInstance().setup();

		// Camera stuff
		Apollo::GameSettings::getInstance().windowCfg->scaleFactor = 2.0f;
		Apollo::OrthographicCamera cam(0.0f, 0.0f, Apollo::GameSettings::getInstance().windowCfg->windowWidth / Apollo::GameSettings::getInstance().windowCfg->scaleFactor, Apollo::GameSettings::getInstance().windowCfg->windowHeight / Apollo::GameSettings::getInstance().windowCfg->scaleFactor, true);
		cam.uploadMatrix(shader, "cameraProjection");

		Apollo::DebugRenderer debugRenderer(cam);

		debugRenderer.init();

		Apollo::Texture::TextureArray blockTextures = getBlockTextures();

		Apollo::Planet planet(128, cam, blockTextures);

		Apollo::Player player(planet, Apollo::Rectangle(0.0f, 0.0f, 30.0f, 40.0f), shader);

		Apollo::BlockManager::getInstance().addBlock("air", 0);
		Apollo::BlockManager::getInstance().addBlock("dirt", blockTextures.getEntry("textures/dirt.png"));
		Apollo::BlockManager::getInstance().addBlock("grass", blockTextures.getEntry("textures/grass.png"));
		Apollo::BlockManager::getInstance().addBlock("chest", blockTextures.getEntry("textures/chest.png"));

		Apollo::EarthWorldGenerator worldGenerator(50);

		planet.setBlockAt(Apollo::BlockPos(15, 15), Apollo::BlockManager::getInstance().getBlock(2));

		for (int x = 0; x < 128; x++)
		{
			int y = worldGenerator.getTopBlockHeightAt(x);
			if (y > 0)
			{
				planet.setBlockAt(Apollo::BlockPos(x, y), Apollo::BlockManager::getInstance().getBlock(2));
				for (int placingY = y - 1; placingY > 0; placingY--)
				{
					planet.setBlockAt(Apollo::BlockPos(x, placingY), Apollo::BlockManager::getInstance().getBlock(1));
				}
			}
		}	

		player.setPos(glm::vec2(100, 500));

		// Setup Platform/Renderer bindings
		ImGuiContext* imGuiContext = Apollo::ApolloImGui::getInstance().init(window);
		ImGui::SetCurrentContext(imGuiContext);

		bool renderTestWindow = true;

		int currentlyPlacingBlock = 0;

		int oof = 0;

		while (!window.shouldClose())
		{
			window.updateWindow();
			window.swapBuffers();
			glClearColor(0.3f, 0.9f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			Apollo::GameSettings::getInstance().gameTime->newFrame();
			Apollo::ApolloImGui::getInstance().newFrame();
			
			if (renderTestWindow)
				ImGui::ShowDemoWindow(&renderTestWindow);
			
			{ // Block selection window
				ImGui::Begin("Place Block", nullptr, ImGuiWindowFlags_AlwaysAutoResize); // Creating a window called "Place Block"
			
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

			planet.render();

			float mouseX = (window.getMouseX() / Apollo::GameSettings::getInstance().windowCfg->scaleFactor) + cam.xOffset();
			float mouseY = ((Apollo::GameSettings::getInstance().windowCfg->windowHeight - window.getMouseY()) / Apollo::GameSettings::getInstance().windowCfg->scaleFactor) + cam.yOffset();
			int mouseBlockX = floor(mouseX / 16.0f);
			int mouseBlockY = floor(mouseY / 16.0f);
			debugRenderer.addSquare(mouseBlockX * 16.0f, mouseBlockY  * 16.0f, 16, 16);

			Apollo::BlockPos translatedMousePos = planet.translateToPlanetPos(Apollo::BlockPos(mouseBlockX, mouseBlockY));
			
			if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
			{
				if (translatedMousePos.x >= 0 && translatedMousePos.y >= 0)
				{
					if (planet.getWorld().chunkExistsAt(translatedMousePos))
					{
						Apollo::Block block = planet.getWorld().getBlock(translatedMousePos);
						if (block.blockID() == 3)
						{
							AP_INFO("{}", static_cast<Apollo::TileEntityChest*>(planet.getWorld().getTileEntity(translatedMousePos))->getStoredValue());
						}
						if (block.blockID() != 0)
							planet.getWorld().setBlock(translatedMousePos, Apollo::BlockManager::getInstance().getBlock(0));
					}
				}
			}
			if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) && !ImGui::GetIO().WantCaptureMouse)
			{
				if (translatedMousePos.x >= 0 && translatedMousePos.y >= 0)
				{
					if (planet.getWorld().chunkExistsAt(translatedMousePos))
					{
						if (planet.getWorld().getBlock(translatedMousePos).blockID() != currentlyPlacingBlock + 1)
						{
							Apollo::TileEntityChest* tileEntity = nullptr;
							if (currentlyPlacingBlock == 2)
							{
								tileEntity = new Apollo::TileEntityChest(translatedMousePos);
								tileEntity->setStoredValue(oof);
								oof++;
							}
							planet.getWorld().setBlock(translatedMousePos, Apollo::BlockManager::getInstance().getBlock(currentlyPlacingBlock + 1), tileEntity);
						}
					}
					else
					{
						Apollo::TileEntityChest* tileEntity = nullptr;
						if (currentlyPlacingBlock == 2)
						{
							tileEntity = new Apollo::TileEntityChest(translatedMousePos);
							tileEntity->setStoredValue(oof);
							oof++;
						}
						planet.getWorld().setBlock(translatedMousePos, Apollo::BlockManager::getInstance().getBlock(currentlyPlacingBlock + 1), tileEntity);
					}
				}
			}

			debugRenderer.drawAndFlush();

			planet.tick(player.getBlockPos());

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