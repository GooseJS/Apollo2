#pragma once

#include <vector>

#include "Apollo/render/Renderable.h"

namespace Apollo
{
	class SpriteBatch : public Renderable
	{
	private:
		std::vector<Renderable> _batchList;
	public:
		SpriteBatch();
		~SpriteBatch();

		inline void queueForDraw(Renderable& renderable) { _batchList.push_back(renderable); }
		inline void clearDrawQueue() { _batchList.clear(); }

		virtual void render();
	};
}