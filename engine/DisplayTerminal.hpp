#pragma once
#include <string>
#include <Windows.h>
#include "Display.hpp"
#include <queue>

namespace engine {
	struct DisplayContent {
		std::string text;
		int x;
		int y;
		Color color;
	};

	class DisplayTerminal : public Display {
	public:
		int getHeight() override;
		int getWidth() override;

		void show() override;
		void clear() override;

		void drawText(std::string text, int x, int y, Color color) override;

	private:
		std::queue<DisplayContent> queue = std::queue<DisplayContent>();
		COORD getSize();
	};
}