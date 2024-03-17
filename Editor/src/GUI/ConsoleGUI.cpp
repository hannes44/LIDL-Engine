#include "ConsoleGUI.hpp"
#include <Engine.hpp>
#include "../../vendor/ImGuizmo/ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>
#include "Serializer/EditorSerializer.hpp"
#include "Serializer/GameSerializer.hpp"
#include <Physics/GamePhysics.hpp>
#include <memory>
#include <iostream>
#include <Core/Debug.hpp>

namespace engine
{
	void ConsoleGUI::ShowConsole()
	{
		static ConsoleStruct log;

		if (Debug::LogQueue.size() > 0)
		{
			LogEntry logEntry = Debug::LogQueue.front();
			Debug::LogQueue.erase(Debug::LogQueue.begin());

			log.AddLog((logEntry.message + "\n").c_str());
		}

		log.Draw("Example: Log");
	}
}