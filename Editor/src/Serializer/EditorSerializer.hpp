#pragma once

#include <string>
#include "EditorSettings.hpp"
#include <vector>
#include "Project.hpp"

namespace engine
{
	class EditorSerializer
	{
	public:
			EditorSerializer();
			~EditorSerializer();

			static void createYAMLFile(const std::string& filePath, const std::string& fileName);

			static void serializeEditorSettings(EditorSettings& settings);

			static EditorSettings deSerializeEditorSettings();

			static std::vector<std::string> getAllGameNamesInGamesFolder();

			static bool isProjectPathValid(const std::string& path);

			static bool isProjectNameValid(const std::string& name);

			static void createFolder(const std::string& path);

			static std::string addFileFromWindowsExplorerToProject(Project* project, char* fileExplorerFilter);

			static std::string getFolderPathFromFileExplorer(const std::string initialPath = "");
	};
}