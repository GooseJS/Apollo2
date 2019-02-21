#include "Apollo/Window.h"

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

		AP_ENGINE_INFO("Window created successfully.");
		_windowCreated = true;
		return APOLLO_NO_ERROR;
	}
}