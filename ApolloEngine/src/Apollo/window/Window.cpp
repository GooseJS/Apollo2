#include "Apollo/window/Window.h"

namespace Apollo
{
	Window::Window()
	{

	}

	Window::Window(const WindowCfg& windowCfg) : _windowCfg(windowCfg)
	{

	}

	Window::~Window()
	{
		if (_windowCfg.created)
			glfwDestroyWindow(_window);
	}

	int Window::createWindow()
	{
		Apollo::Logger::init();

		if (glfwInit() != GLFW_TRUE)
		{
			// TODO (Brendan): Error handling and actual logging of what went wrong!
			AP_ENGINE_CRITICAL("Could not initialize GLFW! (TODO: Better error logging)");
			return APOLLO_GLFW_ERROR;
		}

		AP_ENGINE_INFO("Creating window...");
		_window = glfwCreateWindow(_windowCfg.windowWidth, _windowCfg.windowHeight, _windowCfg.windowTitle.c_str(), nullptr, nullptr);

		if (_window == nullptr)
		{
			// TODO (Brendan): Actual error logging of what went wrong.
			AP_ENGINE_CRITICAL("Could not create window. (TODO: Actual logging of what went wrong.)");
			return APOLLO_WINDOW_ERROR;
		}

		glfwMakeContextCurrent(_window); 

		if (glewInit() != GLEW_OK)
		{
			// TODO (Brendan): Actual error logging of what went wrong.
			AP_ENGINE_CRITICAL("Could not initalize GLEW. (TODO: Actual error logging of what went wrong.)");
			return APOLLO_GLEW_ERROR;
		}

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			EventWindowResize event;
			EventSystem::getInstance().dispatchEvent(event);
		});

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			EventWindowClose event;
			EventSystem::getInstance().dispatchEvent(event);
		});

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (action)
			{
			case GLFW_PRESS:
			{
				EventKeyPressed event(key, scancode, mods);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				EventKeyReleased event(key, scancode, mods);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			case GLFW_REPEAT:
			{
				EventKeyRepeat event(key, scancode, mods);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			switch (action)
			{
			case GLFW_PRESS:
			{
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				EventMouseButtonPressed event(button, xpos, ypos);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				EventMouseButtonPressed event(button, xpos, ypos);
				EventSystem::getInstance().dispatchEvent(event);
				break;
			}
			}
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
		{

			EventMouseMove event((float)xPos, (float)-yPos, (float)xPos - GameSettings::getInstance().internalData->oldMouseX, GameSettings::getInstance().internalData->oldMouseY - (float)yPos);
			EventSystem::getInstance().dispatchEvent(event);

			GameSettings::getInstance().internalData->oldMouseX = (float)xPos;
			GameSettings::getInstance().internalData->oldMouseY = (float)yPos;
		});

		EventWindowOpen event;
		EventSystem::getInstance().dispatchEvent(event);

		AP_ENGINE_INFO("Window created successfully.");
		_windowCreated = true;
		GameSettings::getInstance().windowCfg = &_windowCfg;
		return APOLLO_NO_ERROR;
	}
}