#include "Display.hpp"
#include "DisplayTerminal.hpp"
#include <stdlib.h>
#include <windows.h>
#include <iostream>

namespace engine {

	COORD DisplayTerminal::getSize() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD size;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		return size;
	}

	int DisplayTerminal::getHeight() {
		return getSize().Y;
	}

	int DisplayTerminal::getWidth() {
		return getSize().X;
	}

	void DisplayTerminal::show() {
		while (!queue.empty()) {
			auto content = queue.front();

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), content.color);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (short)content.x, (short)content.y });
			std::cout << content.text << std::endl;

			queue.pop();
		}
	}

	void DisplayTerminal::clear() {
		system("cls");
	}

	void DisplayTerminal::drawText(std::string text, int x, int y, Color color = Color::WHITE) {
		queue.push({ text, x, y, color });
	}
}