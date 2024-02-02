#pragma once
#include <string>
#include "Core/GameObject.hpp"

namespace engine
{
	enum Color {
		BLUE = 1,
		GREEN = 2,
		CYAN = 3,
		RED = 4,
		PURPLE = 5,
		YELLOW = 6,
		WHITE = 7,
		GRAY = 8,
		LIGHT_BLUE = 9,
		LIGHT_GREEN = 10,
		LIGHT_CYAN = 11,
		LIGHT_RED = 12,
		LIGHT_PURPLE = 13,
		LIGHT_YELLOW = 14,
		LIGHT_WHITE = 15
	};

	class Display
	{
	public:
		virtual int getHeight() = 0;
		virtual int getWidth() = 0;

		virtual void show() = 0;
		virtual void clear() = 0;

		virtual void drawText(std::string text, int x, int y, Color color) = 0;
		virtual void drawGameObject(GameObject* gameObject) = 0;
	};
}