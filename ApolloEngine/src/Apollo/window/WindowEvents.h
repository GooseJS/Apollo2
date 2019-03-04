#pragma once

#include "Apollo/GL.h"
#include "Apollo/Core.h"
#include "Apollo/event/IEvent.h"

namespace Apollo
{
	struct EventWindowOpen : public IEvent
	{
	public:
		EventWindowOpen() {}
	};

	struct EventWindowResize : public IEvent
	{
	public:
		EventWindowResize() {}
	};

	struct EventWindowClose : public IEvent
	{
	public:
		EventWindowClose() {}
	};
}
