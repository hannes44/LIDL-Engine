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

		if (ImGui::SmallButton("[Debug] Add 5 entries"))
		{
			static int counter = 0;
			const char* categories[3] = { "info", "warn", "error" };
			const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
			for (int n = 0; n < 5; n++)
			{
				const char* category = categories[counter % IM_ARRAYSIZE(categories)];
				const char* word = words[counter % IM_ARRAYSIZE(words)];
				log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
					ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
				counter++;
			}
		}

		if (ImGui::SmallButton("[Test] Hello World!"))
		{
			std::string message = "Hello World!";
			Debug::Log(message);
		}

		if (Debug::LogQueue.size() > 0)
		{
			LogEntry logEntry = Debug::LogQueue.front();
			Debug::LogQueue.erase(Debug::LogQueue.begin());

			log.AddLog((logEntry.message + "\n").c_str());
		}

		log.Draw("Example: Log");
	}
}