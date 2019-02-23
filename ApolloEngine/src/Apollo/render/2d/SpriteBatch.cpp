#include "Apollo/render/2d/SpriteBatch.h"

namespace Apollo
{
	SpriteBatch::SpriteBatch()
	{
		
	}

	SpriteBatch::~SpriteBatch()
	{
		
	}

	void SpriteBatch::prepare(std::string shaderFiles)
	{

		_shader.initFromFile(shaderFiles.append(".vert"), shaderFiles.append(".frag")); // TODO (Brendan): Stop being a dumbass and implement some dope ass functionality like this into the shader utility class
	}

	void SpriteBatch::prepare(Shader shader)
	{
		_shader = shader;
	}

	void SpriteBatch::flushBatch(Texture::TextureArray textureArray)
	{
		if (!_haveVAO)
		{
			glGenVertexArrays(1, &_batchVAO);
			glBindVertexArray(_batchVAO);

			glGenBuffers(1, &_batchVBO);


		}

		_shader.use();
		glActiveTexture(textureArray.textureID);
	}
}