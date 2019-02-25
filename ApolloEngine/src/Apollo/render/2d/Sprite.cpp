#include "Apollo/render/2d/Sprite.h"

namespace Apollo
{
	void Sprite::init(float width, float height, GLuint textureID, bool useTextureArray)
	{
		_pos.x = 0;
		_pos.y = 0;

		glGenVertexArrays(1, &_vaoID);
		glBindVertexArray(_vaoID);

		GLuint vbo;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		std::vector<glm::vec2> vertices;

		vertices.emplace_back(0,		0);			// 0, 0 | 0, 1
		vertices.emplace_back(width,	height);	// 1, 1 | 1, 0
		vertices.emplace_back(0,		height);	// 0, 1 | 0, 0
		
		vertices.emplace_back(width,	height);	// 1, 1 | 1, 0
		vertices.emplace_back(0,		0);			// 0, 0 | 0, 1
		vertices.emplace_back(width,	0);			// 1, 0 | 1, 1

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0][0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);

		_textureID = textureID;
		_useTextureArray = useTextureArray;

		_transformMatrix = glm::mat4(1.0f);
	}

	void Sprite::draw()
	{
		glBindVertexArray(_vaoID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Sprite::updateTransform()
	{
		_transformMatrix = glm::mat4(1.0f);
		_transformMatrix = glm::translate(_transformMatrix, glm::vec3(_pos, 0.0f));
	}

	void Sprite::setPos(glm::vec2 vec)
	{
		_pos = vec;
	}

	void Sprite::move(glm::vec2 vec)
	{
		_pos += vec;
	}

	void Sprite::move(float x, float y)
	{
		move(glm::vec2(x, y));
	}
}