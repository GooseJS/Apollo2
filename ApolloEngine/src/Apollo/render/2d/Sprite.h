#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Apollo/GL.h"
#include "Apollo/Core.h"
#include "Apollo/logger/Logger.h"
#include "Apollo/render/Texture.h"

namespace Apollo
{
	class APOLLO_API Sprite
	{
	private:
		bool _init;

		GLuint _vaoID;
		GLuint _textureID;

		bool _useTextureArray = false;

		glm::vec2 _pos;

		glm::mat4 _transformMatrix;

		void updateTransform();
	public:
		void init(float width, float height, GLuint textureID, bool useTextureArray = false);

		void draw();

		void move(float x, float y);

		void setPos(glm::vec2 vec);
		void move(glm::vec2 vec);

 		inline glm::mat4 getTransformMatrix() { updateTransform();  return _transformMatrix; }
	};
}