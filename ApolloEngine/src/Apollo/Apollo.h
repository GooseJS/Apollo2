#pragma once

#include <string>

struct GLFWwindow;

namespace Apollo
{
	struct WindowCfg
	{
		int windowWidth = 1280;
		int windowHeight = 720;
		std::string windowTitle = "Apollo Engine";

		bool created = false;

		float scaleFactor = 1.0f;

		GLFWwindow* glfwWindow;
	};

	struct OpenGLConfig
	{
		int glVersionMajor = 3;
		int glVersionMinor = 2;

		bool backwardCompat = true;
	};
}