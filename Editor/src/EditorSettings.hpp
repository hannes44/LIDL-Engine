#pragma once
#include <Engine.hpp>
#include <vector>
#include <Serializer/Serializable.hpp>

namespace engine
{

	class EditorSettings : public Serializable
	{
	public:
		bool useDarkTheme = true;

		bool showGizmos = true;

		// If this is turned on, the editor will save the scene to a file when the application is closed
		// and load it when the application is started. 
		// If this is turned off, the editor run the game initialization code to load the initial scene.
		bool useSerialization = true;

		bool enableUserRetention = false;

		bool enableScripting = true;
		
		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return 
			{
				{SerializableType::BOOLEAN, "Use Dark Theme", "Use Dark theme for the editor", &useDarkTheme},
				{SerializableType::BOOLEAN, "Show Gizmos", "Show Gizmos", &showGizmos},
				{SerializableType::BOOLEAN, "Use Serialization", "Should the editor load the inital gamestate from save file or from initiation function. With this turned of, the editor state will not save", &useSerialization},
				{SerializableType::BOOLEAN, "Enable User Retention", "Shows the user retention panel", &enableUserRetention},
				{SerializableType::BOOLEAN, "Enable Scripting", "Enables C# and Lua scripting", &enableScripting}
			};
		};
		
		RendererSettings rendererSettings{};
	};
}