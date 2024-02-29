#include "Display.hpp"
#include "DisplayTerminal.hpp"
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <algorithm>

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

	void DisplayTerminal::setCursorPosition(short x, short y) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
	}

	void DisplayTerminal::show() {
		std::sort(objects.begin(), objects.end());

		while (!objects.empty()) {
			auto content = objects.back();

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), content->color);

			auto repr = content->getRepresentation();
			for (auto it = repr.begin(); it != repr.end(); it++) {
				setCursorPosition(it->first.x, it->first.y);
				std::cout << it->second;
			}

			objects.pop_back();
		}

		setCursorPosition(0, 0);
	}

	void DisplayTerminal::clear() {
		system("cls");
	}

	void DisplayTerminal::drawText(std::string text, int x, int y, Color color = Color::WHITE) {
		auto* content = new DisplayTextContent(text, x, y, 0, color);
		objects.push_back(content);
	}

	void DisplayTerminal::drawGameObject(GameObject* gameObject) {
		glm::vec3 position = gameObject->getGlobalTransform().getPosition();
		auto content = new DisplayGameObjectContent(gameObject, position.x, position.y, position.z, Color::BLUE);
		objects.push_back(content);
	}
}