#pragma once

#include "Apollo/Core.h"
#include "Apollo/event/IEvent.h"

namespace Apollo
{
	struct EventKeyPressed : public IEvent
	{
	private:
		const int _key;
		const int _scancode;
		const int _mods;
	public:
		EventKeyPressed(int key, int scancode, int mods) : _key(key), _scancode(scancode), _mods(mods) {}
		int key() const { return _key; }
		int scancode() const { return _scancode; }
		int mods() const { return _mods; }
	};

	struct EventKeyReleased : public IEvent
	{
	private:
		const int _key;
		const int _scancode;
		const int _mods;
	public:
		EventKeyReleased(int key, int scancode, int mods) : _key(key), _scancode(scancode), _mods(mods) {}
		int key() const { return _key; }
		int scancode() const { return _scancode; }
		int mods() const { return _mods; }
	};

	struct EventKeyRepeat : public IEvent
	{
	private:
		const int _key;
		const int _scancode;
		const int _mods;
	public:
		EventKeyRepeat(int key, int scancode, int mods) : _key(key), _scancode(scancode), _mods(mods) {}
		int key() const { return _key; }
		int scancode() const { return _scancode; }
		int mods() const { return _mods; }
	};

	struct EventMouseButtonPressed : public IEvent
	{
	private:
		const int _button;
		const float _x;
		const float _y;
	public:
		EventMouseButtonPressed(int button, float x, float y) : _button(button), _x(x), _y(y) {}
		int button() const { return _button; }
		float x() const { return _x; }
		float y() const { return _y; }
	};

	struct EventMouseButtonReleased : public IEvent
	{
	private:
		const int _button;
		const float _x;
		const float _y;
	public:
		EventMouseButtonReleased(int button, float x, float y) : _button(button), _x(x), _y(y) {}
		int button() const { return _button; }
		float x() const { return _x; }
		float y() const { return _y; }
	};

	struct EventMouseMove : public IEvent
	{
	private:
		const float _x;
		const float _y;
		const float _deltaX;
		const float _deltaY;
	public:
		EventMouseMove(float x, float y, float deltaX, float deltaY) : _x(x), _y(y), _deltaX(deltaX), _deltaY(deltaY) {}
		float x() const { return _x; }
		float y() const { return _y; }
		float deltaX() const { return _deltaX; }
		float deltaY() const { return _deltaY; }
	};
}