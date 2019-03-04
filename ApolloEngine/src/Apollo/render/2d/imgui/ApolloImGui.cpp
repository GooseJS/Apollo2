#include "Apollo/render/2d/imgui/ApolloImGui.h"

namespace Apollo
{
	namespace ApolloImGui
	{
		// OpenGL Data
		static char         _glslVersionString[32] = "#version 330";
		static GLuint       _fontTexture = 0;
		static Shader		_shader{};
		static int          _attribLocationTex = 0, _attribLocationProjMtx = 0;
		static int          _attribLocationPosition = 0, _attribLocationUV = 0, _attribLocationColor = 0;
		static unsigned int _vboHandle = 0, _elementsHandle = 0;

		// GLFW data
		static Window*		_window;
		static bool			_mouseJustPressed[5] = { false, false, false, false, false };
		static GLFWcursor*	_mouseCursors[ImGuiMouseCursor_COUNT] = { 0 };

		static const char* getClipboardText(void* user_data)
		{
			return glfwGetClipboardString((GLFWwindow*)user_data);
		}

		static void setClipboardText(void* user_data, const char* text)
		{
			glfwSetClipboardString((GLFWwindow*)user_data, text);
		}

		ImGuiContext* init(Window& window)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			ImGuiIO& io = ImGui::GetIO();
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
			io.BackendRendererName = "imgui_apollo";

			io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
			io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
			io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
			io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
			io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
			io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
			io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
			io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
			io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
			io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
			io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
			io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
			io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
			io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
			io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
			io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
			io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
			io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
			io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
			io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
			io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

			io.SetClipboardTextFn = setClipboardText;
			io.GetClipboardTextFn = getClipboardText;
			io.ClipboardUserData = window.getGLFWWindow();

			_mouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
			_mouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
			_mouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
			_mouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
			_mouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
			_mouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
			_mouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
			_mouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

			_window = &window;

			return ImGui::GetCurrentContext();
		}

		void shutdown()
		{
			for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
			{
				glfwDestroyCursor(_mouseCursors[cursor_n]);
				_mouseCursors[cursor_n] = nullptr;
			}
			destroyDeviceObjects();
		}

		void newFrame()
		{
			if (!_fontTexture)
				createDeviceObjects();

			ImGuiIO& io = ImGui::GetIO();

			int w, h;
			int displayWidth, displayHeight;
			glfwGetWindowSize(_window->getGLFWWindow(), &w, &h);
			glfwGetFramebufferSize(_window->getGLFWWindow(), &displayWidth, &displayHeight);
			io.DisplaySize = ImVec2((float)w, (float)h);
			io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)displayWidth / w) : 0, h > 0 ? ((float)displayHeight / h) : 0);

			io.DeltaTime = GameSettings::getInstance().gameTime->getDeltaTime();

			ImGui::NewFrame();

			for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
			{
				io.MouseDown[i] = _mouseJustPressed[i] || _window->isMouseButtonPressed(i);
				_mouseJustPressed[i] = false;
			}

			// Update mouse position
			const ImVec2 mousePosBackup = io.MousePos;
			io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
			const bool focused = glfwGetWindowAttrib(_window->getGLFWWindow(), GLFW_FOCUSED) != 0;
			if (focused)
			{
				if (io.WantSetMousePos)
					glfwSetCursorPos(_window->getGLFWWindow(), (double)mousePosBackup.x, (double)mousePosBackup.y);
				else
					io.MousePos = ImVec2(_window->getMouseX(), _window->getMouseY());
			}

			if (!((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(_window->getGLFWWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED))
			{
				ImGuiMouseCursor imguiCursor = ImGui::GetMouseCursor();
				if (imguiCursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
				{
					glfwSetInputMode(_window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				}
				else
				{
					glfwSetCursor(_window->getGLFWWindow(), _mouseCursors[imguiCursor] ? _mouseCursors[imguiCursor] : _mouseCursors[ImGuiMouseCursor_Arrow]);
					glfwSetInputMode(_window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
			}
		}

		bool createFontsTextures()
		{
			// Creating texture atlas
			ImGuiIO& io = ImGui::GetIO();
			unsigned char* pixels;
			int width, height;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

			// Upload texture to graphics card
			GLint lastTexture;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
			glGenTextures(1, &_fontTexture);
			glBindTexture(GL_TEXTURE_2D, _fontTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			// Storing the textureID;
			io.Fonts->TexID = (ImTextureID)(intptr_t)_fontTexture;

			// Restoring state
			glBindTexture(GL_TEXTURE_2D, lastTexture);

			return true; // TODO: Maybe some error checking?
		}

		void destroyFontsTextures()
		{
			if (_fontTexture)
			{
				ImGuiIO& io = ImGui::GetIO();
				glDeleteTextures(1, &_fontTexture);
				io.Fonts->TexID = 0;
				_fontTexture = 0;
			}
		}

		void render()
		{
			ImGui::Render();
			ImDrawData* drawData = ImGui::GetDrawData();
			// Checking that there is something for us to render to, and also scaling for retina (using the frame buffer size, not screen size)
			int width = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
			int height = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
			if (width <= 0 || height <= 0)
				return;

			// Save the previous GL state
			GLint lastProgram; glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
			GLenum lastActiveTexture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&lastActiveTexture);
			glActiveTexture(GL_TEXTURE0);
			GLint lastTexture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
			GLint lastArrayBuffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
			GLint lastVertexArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);
			GLint lastViewport[4]; glGetIntegerv(GL_VIEWPORT, lastViewport);
			GLint lastScissorBox[4]; glGetIntegerv(GL_SCISSOR_BOX, lastScissorBox);
			GLenum lastBlendSrcRGB; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&lastBlendSrcRGB);
			GLenum lastBlendDstRGB; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&lastBlendDstRGB);
			GLenum lastBlendSrcAlpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&lastBlendSrcAlpha);
			GLenum lastBlendDstAlpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&lastBlendDstAlpha);
			GLenum lastBlendEquationRGB; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&lastBlendEquationRGB);
			GLenum lastBlendEquationAlpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&lastBlendEquationAlpha);
			GLboolean lastEnableBlend = glIsEnabled(GL_BLEND);
			GLboolean lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
			GLboolean lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
			GLboolean lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);

			// Setup render: enable alpha-blending, no face culling, no depth testing, and scissor enabled
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_SCISSOR_TEST);

			glViewport(0, 0, (GLsizei)width, (GLsizei)height);
			float left = drawData->DisplayPos.x;
			float right = drawData->DisplayPos.x + drawData->DisplaySize.x;
			float top = drawData->DisplayPos.y;
			float bottom = drawData->DisplayPos.y + drawData->DisplaySize.y;

			glm::mat4 orthoMatrix = glm::ortho(left, right, bottom, top);

			_shader.use();
			glUniform1i(_attribLocationTex, 0);
			glUniformMatrix4fv(_attribLocationProjMtx, 1, GL_FALSE, &orthoMatrix[0][0]);

			// Recreate this VAO every time
			// (Apparently it is to easily allow multiple GL contexts, but IDK if i care about that???)
			GLuint vaoHandle = 0;
			glGenVertexArrays(1, &vaoHandle);
			glBindVertexArray(vaoHandle);
			glBindBuffer(GL_ARRAY_BUFFER, _vboHandle);
			glEnableVertexAttribArray(_attribLocationPosition);
			glEnableVertexAttribArray(_attribLocationUV);
			glEnableVertexAttribArray(_attribLocationColor);
			glVertexAttribPointer(_attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
			glVertexAttribPointer(_attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
			glVertexAttribPointer(_attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

			// Will project scissor / clipping rectangles into framebuffer space
			ImVec2 clipOff = drawData->DisplayPos;			// (0, 0) unless using multi-viewports
			ImVec2 clipScale = drawData->FramebufferScale;	// (1, 1) unless using retina display which are often (2, 2)

			// Render command lists
			for (int n = 0; n < drawData->CmdListsCount; n++)
			{
				const ImDrawList* cmdList = drawData->CmdLists[n];
				size_t idxBufferOffset = 0;

				glBindBuffer(GL_ARRAY_BUFFER, _vboHandle);
				glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmdList->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmdList->VtxBuffer.Data, GL_STREAM_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementsHandle);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmdList->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmdList->IdxBuffer.Data, GL_STREAM_DRAW);

				for (int cmd_i = 0; cmd_i < cmdList->CmdBuffer.Size; cmd_i++)
				{
					const ImDrawCmd* pcmd = &cmdList->CmdBuffer[cmd_i];
					if (pcmd->UserCallback)
					{
						// We have a user callback (registered vai ImDrawList::AddCallback)
						pcmd->UserCallback(cmdList, pcmd);
					}
					else
					{
						// Project scissor / clipping rectangle into framebuffer space
						ImVec4 clipRect;
						clipRect.x = (pcmd->ClipRect.x - clipOff.x) * clipScale.x;
						clipRect.y = (pcmd->ClipRect.y - clipOff.y) * clipScale.y;
						clipRect.z = (pcmd->ClipRect.z - clipOff.x) * clipScale.x;
						clipRect.w = (pcmd->ClipRect.w - clipOff.y) * clipScale.y;

						if (clipRect.x < width && clipRect.y < height && clipRect.z >= 0.0 && clipRect.w >= 0.0f)
						{
							// Apply scissor / clipping rectangle
							glScissor((int)clipRect.x, (int)(height - clipRect.w), (int)(clipRect.z - clipRect.x), (int)(clipRect.w - clipRect.y));

							// Bind the texture, and draw
							glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
							glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)idxBufferOffset);
						}
					}
					idxBufferOffset += pcmd->ElemCount * sizeof(ImDrawIdx);
				}
			}
			glDeleteVertexArrays(1, &vaoHandle);

			// Restore GL state
			glUseProgram(lastProgram);
			glBindTexture(GL_TEXTURE_2D, lastTexture);
			glActiveTexture(lastActiveTexture);
			glBindVertexArray(lastVertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
			glBlendEquationSeparate(lastBlendEquationRGB, lastBlendEquationAlpha);
			glBlendFuncSeparate(lastBlendSrcRGB, lastBlendDstRGB, lastBlendSrcAlpha, lastBlendDstAlpha);
			if (lastEnableBlend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
			if (lastEnableCullFace) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
			if (lastEnableDepthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
			if (lastEnableScissorTest) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
			glViewport(lastViewport[0], lastViewport[1], lastViewport[2], lastViewport[3]);
			glScissor(lastScissorBox[0], lastScissorBox[1], lastScissorBox[2], lastScissorBox[3]);
		}

		bool createDeviceObjects()
		{
			// Backup GL state
			GLint lastTexture, lastArrayBuffer, lastVertexArray;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);

			// Load Shader
			const GLchar* vertexShaderSource =
				"#version 130\n"
				"uniform mat4 ProjMtx;\n"
				"in vec2 Position;\n"
				"in vec2 UV;\n"
				"in vec4 Color;\n"
				"out vec2 Frag_UV;\n"
				"out vec4 Frag_Color;\n"
				"void main()\n"
				"{\n"
				"    Frag_UV = UV;\n"
				"    Frag_Color = Color;\n"
				"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
				"}\n";

			const GLchar* fragmentShaderSource =
				"#version 130\n"
				"uniform sampler2D Texture;\n"
				"in vec2 Frag_UV;\n"
				"in vec4 Frag_Color;\n"
				"out vec4 Out_Color;\n"
				"void main()\n"
				"{\n"
				"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
				"}\n";

			_shader.initFromString(vertexShaderSource, fragmentShaderSource);

			_attribLocationTex = _shader.uniform("Texture");
			_attribLocationProjMtx = _shader.uniform("ProjMtx");
			_attribLocationPosition = _shader.attribute("Position");
			_attribLocationUV = _shader.attribute("UV");
			_attribLocationColor = _shader.attribute("Color");

			// Create Buffers
			glGenBuffers(1, &_vboHandle);
			glGenBuffers(1, &_elementsHandle);

			createFontsTextures();

			// Restore GL State
			glBindTexture(GL_TEXTURE_2D, lastTexture);
			glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
			glBindVertexArray(lastVertexArray);

			return true;
		}

		void destroyDeviceObjects()
		{
			if (_vboHandle) _vboHandle = _elementsHandle = 0;

			destroyFontsTextures();
		}

		// Event / GLFW callbacks
		void mouseButtonEvent(MouseButtonEvent event)
		{
			if (event.action() == GLFW_PRESS && event.button() >= 0 && event.button() < IM_ARRAYSIZE(_mouseJustPressed))
				_mouseJustPressed[event.button()] = true;
		}

		void scrollEvent(EventMouseScroll event)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseWheelH += event.x();
			io.MouseWheel += event.y();
		}

		void keyboardEvent(KeyboardKeyEvent event)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (event.action() == GLFW_PRESS)
				io.KeysDown[event.key()] = true;
			if (event.action() == GLFW_RELEASE)
				io.KeysDown[event.key()] = false;

			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
			io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		}

		void characterEvent(GLFWwindow* window, unsigned int c)
		{

		}
	}
}