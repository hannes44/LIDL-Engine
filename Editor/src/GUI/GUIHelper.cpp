#include "GUIHelper.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "Renderer/OpenGL/OpenGLGraphicsAPI.hpp"
#include "Core/Texture.hpp"

namespace engine
{
	GUIHelper::GUIHelper() : window(Window::getInstance()) {
	}

	GUIHelper& GUIHelper::getInstance() {

		if (instance == nullptr) {
			instance = new GUIHelper();
		}
		return *instance;
	}

	void GUIHelper::newFrame() {

		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void GUIHelper::endFrame() {

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.newFrame();
	}

	// Draw text in the center of current widget/window
	void GUIHelper::drawCenteredText(const std::string& text) {

		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}

	// Draw a help marker (question mark) with a tooltip
	void GUIHelper::HelpMarker(const char* desc) {

        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

	// Draw text at a specific window position (no widget)
	void GUIHelper::drawText(float xpos, float ypos, const std::string& text, float R, float G, float B, float A) {

		int w, h;
		window.getWindowSize(&w, &h);

		ImGui::GetBackgroundDrawList()->AddText(ImVec2(xpos*w, ypos*h), ImColor(R, G, B, A), text.c_str());
	}

	// Is this function necessary?
	void GUIHelper::drawButton(float xpos, float ypos, float width, float height, const std::string& text) {

		int w, h;
		window.getWindowSize(&w, &h);

		//ImGui::GetBackgroundDrawList()->Add ?
	}

	// Draw an image at a specific window position (no widget)
	// The image is drawn with the top-left corner at the specified position
	void GUIHelper::drawImage(float xpos, float ypos, const std::string& path, float width, float height) {

		int w, h;
		window.getWindowSize(&w, &h);

		std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(Texture::create(path, true));

		ImGui::GetBackgroundDrawList()->AddImage((void*)(intptr_t)texture->textureIDOpenGL, ImVec2(w*ypos,h*xpos), ImVec2((w*ypos)+width,(h*xpos)-height), ImVec2(0,0), ImVec2(1, 1)); // Fix position and UVs
	}
	
}

