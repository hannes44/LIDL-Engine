#pragma once
#include <string>
#include "Display.hpp"
#include "GameObject.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <Windows.h>

namespace engine {

    constexpr auto TERMINAL_GAMEOBJECT_CHAR = 219U;

	struct Point {
		int x;
		int y;

		// Sort based on y first, then x
		// The y axis does not matter for printing, but creates a nicer order
		// The x axis is important, as printing a character one position to the left of a previously printed character
		// will overwrite that previous character
		bool operator < (const Point& point) const
		{
			return (y < point.y) || (!(point.y < y) && x < point.x);
		}
	};

	struct DisplayContent {
		int x;
		int y;
		int z;
		Color color;
		virtual std::map<Point, char> getRepresentation() = 0;

		bool operator < (const DisplayContent& content) const
		{
			return (z < content.z);
		}
	};

	struct DisplayTextContent : public DisplayContent {
		std::string text;
		std::map<Point, char> getRepresentation() override {
			auto map = std::map<Point, char>();
			int offset = 0;

			for (char c : text) {
				map.insert(std::pair<Point, char>({ x + offset, y }, c));
				offset++;
			}

			return map;
		}

		DisplayTextContent(std::string text, int x, int y, int z, Color color) : DisplayContent(), text(std::move(text)) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->color = color;
		}
	};

	struct DisplayGameObjectContent : public DisplayContent {
		GameObject* gameObject;
		std::map<Point, char> getRepresentation() override {
			auto map = std::map<Point, char>();

			for (int i = 0; i < gameObject->transform.getScale().y; i++) {
				for (int j = 0; j < gameObject->transform.getScale().x; j++) {
					map.insert(std::pair<Point, char>({ x + j, y + i }, (char)TERMINAL_GAMEOBJECT_CHAR));
				}
			}

			return map;
		}

		DisplayGameObjectContent(GameObject* gameObject, int x, int y, int z, Color color) : DisplayContent(), gameObject(gameObject) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->color = color;
		}
	};

	class DisplayTerminal : public Display {
	public:
		int getHeight() override;
		int getWidth() override;

		void show() override;
		void clear() override;

		void drawText(std::string text, int x, int y, Color color) override;
		void drawGameObject(GameObject* gameObject) override;

	private:
		std::vector<DisplayContent*> objects = std::vector<DisplayContent*>();
		COORD getSize();
		void setCursorPosition(short x, short y);
	};
}