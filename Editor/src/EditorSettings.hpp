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

		
		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return 
			{
				{SerializableType::BOOLEAN, "Use Dark Theme", "", &useDarkTheme},
				{SerializableType::BOOLEAN, "Show Gizmos", "", &showGizmos},
				{SerializableType::BOOLEAN, "Use Serialization", "", &useSerialization}
			};
		};
		
		RendererSettings rendererSettings{};
	};
}