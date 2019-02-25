#pragma once

#include <chrono>

#include "Apollo/GL.h"
#include "Apollo/Core.h"

#define APOLLO_GET_CURRENT_TIME_MILLIS std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define APOLLO_GET_CURRENT_TIME_SECONDS std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()

namespace Apollo
{
	class APOLLO_API ApolloTime
	{
	private:
		double _deltaTime = 0;
		double _lastFrameTime = 0;
		double _currentFrameTime =  0;
	public:
		ApolloTime() { _currentFrameTime = glfwGetTime(); _deltaTime = _currentFrameTime; _lastFrameTime = _currentFrameTime; }

		double getLastFrameTime() const { return _lastFrameTime; }
		double getCurrentFrameTime() const { return _currentFrameTime; }
		float getDeltaTime() const { return _deltaTime; }
		double getCurrentTimeInMS() const { return APOLLO_GET_CURRENT_TIME_MILLIS; }

		inline void newFrame(double glfwTime)
		{
			_currentFrameTime = glfwGetTime();
			_deltaTime = _currentFrameTime - _lastFrameTime;
			_lastFrameTime = _currentFrameTime;
		}
	};

	class APOLLO_API GameSettings
	{
	private:
		GameSettings() {}
		~GameSettings() = default;
		GameSettings(const GameSettings& gs) {}
		GameSettings& operator=(const GameSettings& gs) = delete;
	public:
		static void setup()
		{
			getInstance().gameTime = new ApolloTime();
		}
		WindowCfg* windowCfg;

		static GameSettings& getInstance()
		{
			static GameSettings instance;
			return instance;
		}

		static void cleanup()
		{
			delete getInstance().gameTime;
		}

		ApolloTime* gameTime;
	};
}