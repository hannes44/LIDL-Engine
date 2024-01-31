#pragma once
#include <string>

namespace engine
{
	class Editor
	{
	public:
		void start();

		static void createNewProject(const std::string& name, const std::string& path);

		static void openProject();
	};
}