#pragma once

#include "Core/BoundingBox.hpp"
#include "Components/CameraComponent.hpp"
#include <string>
#define _WINSOCKAPI_
#include <Windows.h>
#include <atlstr.h>

#include <chrono>

namespace engine
{
	class Utils
	{
	public:
		static long long getTimestampNS();

		// [in] command to execute
		static CStringA ExecCmd(const wchar_t* cmd);

		static void drawBoundingBox(BoundingBox& box, CameraComponent* camera, glm::vec3 color);
	};
}