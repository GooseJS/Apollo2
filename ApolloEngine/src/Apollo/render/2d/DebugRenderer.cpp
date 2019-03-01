#include "Apollo/render/2d/DebugRenderer.h"

namespace Apollo
{

	void DebugRenderer::init(glm::mat4 cameraMatrix)
	{
		_cameraMatrix = cameraMatrix;
		_shader.initFromFile("debugShader.vert", "debugShader.frag"); // TODO: Look into other ways of storing these shaders, dont want the end use to need to have .vert and .frag files
		_shader.uniform("cameraProjection", _cameraMatrix); // TODO: This needs to be updated!
		glGenVertexArrays(1, &_vaoID);
		glBindVertexArray(_vaoID);

		glGenBuffers(1, &_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugRendererVertex), (GLvoid*)offsetof(DebugRendererVertex, xy));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DebugRendererVertex), (GLvoid*)offsetof(DebugRendererVertex, color));

	}

	void DebugRenderer::addVertex(float x, float y, float r, float g, float b, float a, bool outlineRender)
	{
		if (outlineRender)
			_outlineVertices.emplace_back(x, y, r, g, b, a);
		else
			_shapeVertices.emplace_back(x, y, r, g, b, a);
	}

	void DebugRenderer::addLine(float x, float y, float x1, float y1, float r, float g, float b, float a)
	{
		addVertex(x, y, r, g, b, a, true);
		addVertex(x1, y1, r, g, b, a, true);
	}

	void DebugRenderer::addTriangle(float x, float y, float x1, float y1, float x2, float y2, float r, float g, float b, float a, bool outlineRender)
	{
		if (!outlineRender)
		{
			addVertex(x, y, r, g, b, a, outlineRender);
			addVertex(x1, y1, r, g, b, a, outlineRender);
			addVertex(x2, y2, r, g, b, a, outlineRender);
		}
	}

	void DebugRenderer::addSquare(float x, float y, float w, float h, float r, float g, float b, float a, bool outlineRender)
	{
		if (!outlineRender)
		{
			addTriangle(x, y, x + w, y, x + w, y + h, r, g, b, a, outlineRender);
			addTriangle(x + w, y + h, x, y + h, x, y, r, g, b, a, outlineRender);
		}
		else
		{
			float lineWidth = 3.f;
			float transX = x - (lineWidth / 2.f);
			float transY = y - (lineWidth / 2.f);
			float transW = w + (lineWidth / 2.f);
			float transH = h + (lineWidth / 2.f);
			addSquare(transX, transY, transW, lineWidth, r, g, b, a);
			addSquare(transX + transW, transY, lineWidth, transH, r, g, b, a);
			addSquare(transX, transY + transH, transW, lineWidth, r, g, b, a);
			addSquare(transX, transY, lineWidth, transH, r, g, b, a);
		}
	}

	void DebugRenderer::drawAndFlush()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(_vaoID);
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		_shader.use();
		if (!_outlineVertices.empty())
		{
			glBufferData(GL_ARRAY_BUFFER, _shapeVertices.size() * sizeof(DebugRendererVertex), &_outlineVertices[0].xy[0], GL_STATIC_DRAW);
			glDrawArrays(GL_LINE_STRIP, 0, _outlineVertices.size());
			_outlineVertices.clear();
		}
		if (!_shapeVertices.empty())
		{
			glBufferData(GL_ARRAY_BUFFER, _shapeVertices.size() * sizeof(DebugRendererVertex), &_shapeVertices[0].xy[0], GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, _shapeVertices.size());
			_shapeVertices.clear();
		}
	}
}