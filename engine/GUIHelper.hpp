#pragma once

#include <string>

namespace engine
{

	class GUIHelper
	{
	public:
		static void drawCenteredText(const std::string& text);

		static void drawTextInput(const std::string& label, std::string& text);
	};
}