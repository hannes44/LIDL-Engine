#pragma once
#include "Camera.hpp"

namespace engine
{
	class Editor
	{
	public:
		Editor();

		void start();

		Camera editorCamera{};
	};
}