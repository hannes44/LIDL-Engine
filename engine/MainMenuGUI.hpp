#pragma once
#include "Project.hpp"
#include <memory>
#include "Window.hpp"

namespace engine
{
	enum class MainMenuState
	{
		NewProject,
		OpenProject,
	};

	enum class ProjectTemplate
	{
		Empty,
		Game2D,
		Game3D,
	};

	class MainMenuGUI
	{
	public:
		std::shared_ptr<engine::Project> MainMenu();

private:
	void renderNewFrame();

	void endFrame();

	void renderMainMenu();

	void renderHeader();

	void renderFooter();

	void renderNewProjectTab();

	void renderOpenProjectTab();

	Window& window = Window::getInstance();

	MainMenuState state = MainMenuState::NewProject;

	const int inputBufferSize = 100;

	inline static char projectName[100] = "New Project";

	inline static char projectPath[100] = "C:/Users/";
	};
}