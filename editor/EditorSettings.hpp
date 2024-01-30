#pragma once
#include "RendererSettings.hpp"

namespace engine
{

	struct EditorSettings
	{
		bool useDarkTheme = true;

		bool showGizmos = true;

		RendererSettings rendererSettings{};
	};
}