#pragma once

#include <string>
#include "EditorSettings.hpp"

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
	};
}