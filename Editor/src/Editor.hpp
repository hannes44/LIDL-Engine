#pragma once
#include <string>
#include "Project.hpp"
#include <memory>

namespace engine
{
	class Editor
	{
	public:
		void start();

		static void createNewProject(const std::string& name, const std::string& path);

		static void openProject();

		std::shared_ptr<Project> project;
	};
}