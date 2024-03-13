#include "Utils.hpp"
#include "Renderer/Renderer.hpp"

namespace engine
{
	long long Utils::getTimestampNS() {
		return std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}

	glm::vec3 Utils::vec3min(glm::vec3 a, glm::vec3 b) {
		return glm::vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}

	glm::vec3 Utils::vec3max(glm::vec3 a, glm::vec3 b) {
		return glm::vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}

	// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
	//
	// Execute a command and get the results. (Only standard output)
	//
	CStringA Utils::ExecCmd(const wchar_t* cmd)
	{
		CStringA strResult;
		HANDLE hPipeRead, hPipeWrite;

		SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
		saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
		saAttr.lpSecurityDescriptor = NULL;

		// Create a pipe to get results from child's stdout.
		if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
			return strResult;

		STARTUPINFOW si = { sizeof(STARTUPINFOW) };
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.hStdOutput = hPipeWrite;
		si.hStdError = hPipeWrite;
		si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing.
		// Requires STARTF_USESHOWWINDOW in dwFlags.

		PROCESS_INFORMATION pi = { 0 };

		BOOL fSuccess = CreateProcessW(NULL, (LPWSTR)cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if (!fSuccess)
		{
			CloseHandle(hPipeWrite);
			CloseHandle(hPipeRead);
			return strResult;
		}

		bool bProcessEnded = false;
		for (; !bProcessEnded;)
		{
			// Give some timeslice (50 ms), so we won't waste 100% CPU.
			bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

			// Even if process exited - we continue reading, if
			// there is some data available over pipe.
			for (;;)
			{
				char buf[1024];
				DWORD dwRead = 0;
				DWORD dwAvail = 0;

				if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
					break;

				if (!dwAvail) // No data available, return
					break;

				DWORD bytesToRead = sizeof(buf) - 1 < dwAvail ? sizeof(buf) - 1 : dwAvail;

				if (!::ReadFile(hPipeRead, buf, bytesToRead, &dwRead, NULL) || !dwRead)
					// Error, the child process might ended
					break;

				buf[dwRead] = 0;
				strResult += buf;
			}
		} //for

		CloseHandle(hPipeWrite);
		CloseHandle(hPipeRead);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return strResult;
	} //ExecCmd

	void Utils::drawBoundingBox(BoundingBox& box, CameraComponent* camera, glm::vec3 color) {
		// The center position of the box
		glm::vec3 centerPos = box.getCenter();

		// The start point, top corner of the box
		glm::vec3 currPos = centerPos + glm::vec3(-box.getExtent().x / 2, box.getExtent().y / 2, -box.getExtent().z / 2);

		// The end point of the line
		glm::vec3 nextPos = currPos + glm::vec3(box.getExtent().x, 0, 0);

		// We will move on the top surface of the box, so we use this offset vector to move to the mirrored bottom point
		glm::vec3 down = glm::vec3(0, -box.getExtent().y, 0);

		// The offsets that take us to all the top surface corners of the box
		std::vector<glm::vec3> nextPointOffsets = {
			glm::vec3(0, 0, box.getExtent().z),
			glm::vec3(-box.getExtent().x, 0, 0),
			glm::vec3(0, 0, -box.getExtent().z),
			glm::vec3(0, 0, 0) // Needed for the last rendering pass, but the actual value is never used
		};

		Renderer* renderer = Renderer::getInstance();
		for (auto nextPointOffset : nextPointOffsets) {
			renderer->drawLine(currPos, nextPos, color, camera); // Top line
			renderer->drawLine(currPos + down, nextPos + down, color, camera); // Bottom line (mirrored from top)
			renderer->drawLine(currPos, currPos + down, color, camera); // Corner, vertically linking top and bottom

			// Update the current position
			currPos = nextPos;

			// Move to the next position
			nextPos += nextPointOffset;
		}
	}

	glm::vec3 Utils::getMouseRayDirection(Window& window, CameraComponent& camera)
	{
		int mouseX, mouseY;
		window.getMousePosition(&mouseX, &mouseY);

		int width, height;
		window.getWindowSize(&width, &height);

		float aspectRatio = float(width) / float(height);

		glm::mat4 projectionMatrix = camera.getProjectionMatrix();

		glm::vec3 rayDirection = glm::vec3(0, 0, 0);

		float x = (2.0f * mouseX) / width - 1.0f;
		float y = 1.0f - (2.0f * mouseY) / height;
		float z = 1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, -1);

		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

		glm::vec4 ray_eye = glm::inverse(projectionMatrix) * ray_clip;

		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

		glm::vec4 temp = glm::inverse(camera.getViewMatrix()) * ray_eye;
		glm::vec3 ray_wor = { temp.x, temp.y, temp.z };
		ray_wor = glm::normalize(ray_wor);
		return ray_wor;
	}

	std::vector<std::shared_ptr<GameObject>> Utils::getAABBGameObjectCollisions(Game* game, glm::vec3 origin, glm::vec3 direction)
	{
		std::vector<std::shared_ptr<GameObject>> collidingGameObjects;

		for (auto& [id, gameObject] : game->getGameObjects()) 
		{
			if (auto meshComponent = gameObject->getComponent<MeshComponent>())
			{
				if (meshComponent->getBoundingBox().isRayIntersecting(origin, direction))
					collidingGameObjects.push_back(gameObject);
			}
		}

		return collidingGameObjects;
	}
}
