#pragma once

#include <string>

namespace Apollo
{
	struct WindowCfg
	{
		int windowWidth = 1280;
		int windowHeight = 720;
		std::string windowTitle = "Apollo Engine";

		bool created = false;
	};

	struct OpenGLConfig
	{
		int glVersionMajor = 3;
		int glVersionMinor = 2;

		bool backwardCompat = true;
	};
}