#pragma once

#include "Core/BoundingBox.hpp"
#include "Components/CameraComponent.hpp"
#include <string>

#define _WINSOCKAPI_
#define NOMINMAX
#include <Windows.h>

#include "glm/glm.hpp"
#include <atlstr.h>
#include "Core/Window.hpp"

#include <chrono>

namespace engine
{
	class Utils
	{
	public:
		static long long getTimestampNS();

		static glm::vec3 vec3min(glm::vec3 a, glm::vec3 b);
		static glm::vec3 vec3max(glm::vec3 a, glm::vec3 b);

		// [in] command to execute
		static CStringA ExecCmd(const wchar_t* cmd);

		static void drawBoundingBox(BoundingBox& box, CameraComponent* camera, glm::vec3 color);

		// Returns the ray direction from the mouse position into the world space from the camera
		static glm::vec3 getMouseRayDirection(Window& window, CameraComponent& camera);

		// Will only check against the mesh AABB, not against colliders
		static std::vector<std::shared_ptr<GameObject>> getAABBGameObjectCollisions(Game* game, glm::vec3 origin, glm::vec3 direction);


	};
}