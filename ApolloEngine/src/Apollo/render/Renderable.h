#pragma once

namespace Apollo
{
	class Renderable
	{
	public:
		Renderable() = default;
		virtual ~Renderable() = 0;
		virtual void render() = 0;
	};
}