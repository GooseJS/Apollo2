#pragma once

#include <string>

#include "Apollo/GL.h"
#include "Apollo/Apollo.h"
#include "Apollo/InputEvents.h"
#include "Apollo/GameSettings.h"
#include "Apollo/logger/Logger.h"
#include "Apollo/event/EventSystem.h"
#include "Apollo/window/WindowEvents.h"

#define APOLLO_NO_ERROR		0
#define APOLLO_GLFW_ERROR	1
#define APOLLO_WINDOW_ERROR 2
#define APOLLO_GLEW_ERROR	3

namespace Apollo
{
	class APOLLO_API Window
	{
	private:
		GLFWwindow* _window = nullptr;

		WindowCfg _windowCfg {};
		bool _windowCreated = false;
	public:
		Window();
		Window(const WindowCfg& windowCfg);
		~Window();

		int createWindow();

		inline void updateWindow() { glfwPollEvents(); }
		inline void swapBuffers() { glfwSwapBuffers(_window); }

		inline bool shouldClose() { return glfwWindowShouldClose(_window) == GLFW_TRUE; }
		inline void setShouldClose() { glfwSetWindowShouldClose(_window, true); }

		inline bool isKeyPressed(int keyCode) { return glfwGetKey(_window, keyCode) != GLFW_RELEASE; }
		inline bool isMouseButtonPressed(int button) { return glfwGetMouseButton(_window, button) != GLFW_RELEASE; }

		inline double getMouseX() { double x = 0, y = 0; glfwGetCursorPos(_window, &x, &y); return x; }
		inline double getMouseY() { double x = 0, y = 0; glfwGetCursorPos(_window, &x, &y); return y; }
	};
}