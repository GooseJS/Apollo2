#include "Apollo/render/2d/DebugRenderer.h"

namespace Apollo
{
	DebugRenderer::DebugRenderer()
	{
		// TODO: Initialize shader
	}

	DebugRenderer::~DebugRenderer()
	{

	}

	void DebugRenderer::addVertex(float x, float y, float r, float g, float b, float a, bool outlineRender)
	{
		if (outlineRender)
			_outlineVertices.emplace_back(x, y, r, g, b, a);
		else
			_shapeVertices.emplace_back(x, y, r, g, b, a);
	}

	void DebugRenderer::addLine(float x, float y, float x1, float y1, float r, float g, float b, float a, bool outlineRender)
	{
		addVertex(x, y, r, g, b, a, outlineRender);
		addVertex(x1, y1, r, g, b, a, outlineRender);
	}

	void DebugRenderer::addSquare(float x, float y, float w, float h, float r, float g, float b, float a, bool outlineRender)
	{
		addLine(x, y, x + w, y, r, g, b, a, outlineRender);
		addLine(x + w, y, x + w, y + h, r, g, b, a, outlineRender);
		addLine(x + w, y + h, x, y + h, r, g, b, a, outlineRender);
		addLine(x, y + h, x, y, r, g, b, a, outlineRender);
	}

	void DebugRenderer::drawAndFlush()
	{
		glBindVertexArray(_vaoID);
		if (!_outlineVertices.empty())
		{
			// TODO: Uploading buffer data!
			glDrawArrays(GL_LINES, 0, _outlineVertices.size());
			_outlineVertices.clear();
		}
		if (!_shapeVertices.empty())
		{
			// TODO: Uploading buffer data!
			glDrawArrays(GL_QUADS, 0, _shapeVertices.size());
			_shapeVertices.clear();
		}
	}
}