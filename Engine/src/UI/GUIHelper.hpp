#pragma once
#include "Core/Window.hpp"
#include <string>


/*
// GUIHelper class
// This class is a helper class for drawing GUI elements
// It is a singleton | use getInstance to get the instance
// To use this class, you need to call newFrame every you want to draw a new frame
// and endFrame at the end of the frame
// Put any ImGui drawing code between newFrame and endFrame
*/

namespace engine
{
	class GUIHelper
	{
	public:
		GUIHelper(const GUIHelper&) = delete;

		~GUIHelper() = default;

		static GUIHelper& getInstance();

		static void newFrame();

		void endFrame();

		void drawCenteredText(const std::string& text);

		static void HelpMarker(const char* desc);

		void drawText(float xpos, float ypos, const std::string& text, float R, float G, float B, float A);

		void drawButton(float xpos, float ypos, float width, float height, const std::string& text);

		void drawImage(float xpos, float ypos, const std::string& path, float width, float height);

	private:
		GUIHelper();

		inline static GUIHelper* instance = nullptr;

		Window& window;
	};
}