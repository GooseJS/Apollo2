#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

#include "Apollo/GL.h"
#include "Apollo/Core.h"
#include "Apollo/InputEvents.h"
#include "Apollo/window/Window.h"
#include "Apollo/render/Shader.h"
#include "Apollo/event/EventSystem.h"
#include "Apollo/window/WindowEvents.h"

namespace Apollo
{
	namespace ApolloImGui
	{
		APOLLO_API ImGuiContext*	init(Window& window);
		APOLLO_API void				shutdown();
		APOLLO_API void				newFrame();
		APOLLO_API void				render();

		// Called by Init/NewFrame/Shutdown
		APOLLO_API bool     createFontsTextures();
		APOLLO_API void     destroyFontsTextures();
		APOLLO_API bool     createDeviceObjects();
		APOLLO_API void     destroyDeviceObjects();

		APOLLO_API void     mouseButtonEvent(MouseButtonEvent event);
		APOLLO_API void     scrollEvent(EventMouseScroll event);
		APOLLO_API void     keyboardEvent(KeyboardKeyEvent event);
		APOLLO_API void     characterEvent(GLFWwindow* window, unsigned int c);
	}
}