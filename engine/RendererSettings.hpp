#pragma once

namespace engine
{
	struct RendererSettings
	{
		bool enableDepthTest = true;
		bool enableFaceCulling = true;

		bool drawWireframe = false;
		bool useMultiSampling = true;

	};
}