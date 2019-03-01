#pragma once

#include <vector>

#include "Apollo/Core.h"
#include "Apollo/GL.h"
#include "Apollo/logger/logger.h"
#include "Apollo/render/Shader.h"

namespace Apollo
{
	class DebugRenderer
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

	private:
		Shader _shader;

		GLuint _vaoID;

		std::vector<DebugRendererVertex> _outlineVertices;
		std::vector<DebugRendererVertex> _shapeVertices;

	public:
		DebugRenderer();
		~DebugRenderer();

		void addVertex(float x, float y, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool outlineRender = false);

		void addLine(float x, float y, float x1, float y1, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool outlineRender = false);

		void addSquare(float x, float y, float w, float h, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool outlineRender = false);

		void drawAndFlush();
	};
}