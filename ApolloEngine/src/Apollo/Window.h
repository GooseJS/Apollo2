#pragma once

#include <string>

#include "Apollo/GL.h"
#include "Apollo/Apollo.h"
#include "Apollo/GameSettings.h"
#include "Apollo/logger/Logger.h"

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
	};
}