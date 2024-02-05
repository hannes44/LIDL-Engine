#pragma once
#include <string>

namespace engine
{
	class GUIHelper
	{
	public:
		static void drawCenteredText(const std::string& text);
		static void HelpMarker(const char* desc);
	};
}