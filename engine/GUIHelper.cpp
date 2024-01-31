#include "GUIHelper.hpp"
#include <imgui.h>

namespace engine
{
	void GUIHelper::drawCenteredText(const std::string& text)
	{
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}
	void GUIHelper::drawTextInput(const std::string& label, std::string& text)
	{

	}
}

