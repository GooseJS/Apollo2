#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Apollo/render/Shader.h"

namespace Apollo
{
	class OrthographicCamera
	{
	private:
		glm::mat4 _currentMatrix;

		float _xOffset;
		float _yOffset;
		float _width;
		float _height;

		bool _bottomLeft;

		inline void calcMatrix()
		{
			if (_bottomLeft)
				_currentMatrix = glm::ortho(_xOffset, _xOffset + _width, _yOffset, _yOffset + _height);
			else
				_currentMatrix = glm::ortho(_xOffset, _xOffset + _width, _yOffset + _height, _yOffset);
		}
	public:
		OrthographicCamera(float x, float y, float width, float height, bool bottomLeft = false)
		{
			_xOffset = x;
			_yOffset = y;
			_width = width;
			_height = height;
			_bottomLeft = bottomLeft;

			calcMatrix();
		}

		inline void move(int x, int y)
		{
			_xOffset += x;
			_yOffset += y;
			calcMatrix();
		}

		inline void setPos(int x, int y)
		{
			_xOffset = x;
			_yOffset = y;
			calcMatrix();
		}

		inline void resize(int width, int height)
		{
			_width = width;
			_height = height;
			calcMatrix();
		}

		inline float xOffset() const { return _xOffset; }
		inline float yOffset() const { return _yOffset; }
		inline float width() const { return _width; }
		inline float height() const { return _height; }
		inline glm::mat4 matrix() const { return _currentMatrix; }
		
		inline void uploadMatrix(Shader& shader, const std::string& uniformName)
		{
			shader.uniform(uniformName, _currentMatrix);
		}
	};
}