 #pragma once

#include <vector>
#include <string>

#include "Apollo/render/Shader.h"
#include "Apollo/render/Texture.h"
#include "Apollo/render/Renderable.h"

namespace Apollo
{
	class SpriteBatch : public Renderable
	{
	private:
		struct BatchObject
		{
			float x;
			float y;
			float x1;
			float y1;
			float texCoord;

			BatchObject() {}
			BatchObject(float _x, float _y, float _x1, float _y1, float _texCoord) : x(_x), y(_y), x1(_x1), y1(_y1), texCoord(_texCoord) {}
		};
		std::vector<BatchObject> _batchList;
		
		Shader _shader;

		GLuint _batchVAO;
		GLuint _batchVBO;
		bool _haveVAO = false;

	public:
		SpriteBatch();
		~SpriteBatch();

		void prepare(std::string shaderFiles);
		void prepare(Shader shader);

		BatchObject prepareForBatch(float x, float y, float width, float height, float texCoord)
		{
			return BatchObject(x, y, width, height, texCoord);
		} 

		inline void queueForDraw(BatchObject batchObject) { _batchList.push_back(batchObject); }
		inline void clearDrawQueue() { _batchList.clear(); }

		virtual void flushBatch(Texture::TextureArray textureArray);
	};
}