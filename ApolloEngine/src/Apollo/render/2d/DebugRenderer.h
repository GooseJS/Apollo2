#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Apollo/Core.h"
#include "Apollo/GL.h"
#include "Apollo/logger/Logger.h"
#include "Apollo/render/Shader.h"
#include "Apollo/render/2d/OrthographicCamera.h"

namespace Apollo
{
	class APOLLO_API DebugRenderer
	{
	public:
		struct DebugRendererVertex
		{
			float xy[2] = { 0, 0 };
			float color[4] = { 0, 0, 0, 0 };

			DebugRendererVertex()
			{

			}

			DebugRendererVertex(float x, float y, float r, float g, float b, float a)
			{
				xy[0] = x;
				xy[1] = y;
				color[0] = r;
				color[1] = g;
				color[2] = b;
				color[3] = a;
			}
		};

		struct Rectangle
		{
			float x = 0;
			float y = 0;
			float width = 0;
			float height = 0;

			Rectangle() {}
			Rectangle(float _x, float _y, float _width, float _height) : x(_x), y(_y), width(_width), height(_height) {}
		};

	private:
		Shader _shader;

		GLuint _vaoID;
		GLuint _vboID;

		std::vector<DebugRendererVertex> _outlineVertices;
		std::vector<DebugRendererVertex> _shapeVertices;

		OrthographicCamera& _camera;

	public:
		DebugRenderer(OrthographicCamera& camera) : _camera(camera) {}
		~DebugRenderer() {}

		void init();
		inline void changeCamera(OrthographicCamera& camera) { _camera = camera; _camera.uploadMatrix(_shader, "cameraProjection"); }

		void addVertex(float x, float y, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool outlineRender = false);

		void addLine(float x, float y, float x1, float y1, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

		void addTriangle(float x, float y, float x1, float y1, float x2, float y2, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool outlineRender = false);

		void addSquare(float x, float y, float w, float h, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool outlineRender = false);

		void drawAndFlush();

		inline void DebugRenderer::addSquare(Rectangle rect, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool outlineRender = false)
		{
			addSquare(rect.x, rect.y, rect.width, rect.height, r, g, b, a, outlineRender);
		}
	};
}