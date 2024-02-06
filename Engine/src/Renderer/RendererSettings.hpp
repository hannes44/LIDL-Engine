#pragma once
#include "Serializer/Serializable.hpp"

namespace engine
{
	struct RendererSettings : public Serializable
	{
		bool enableDepthTest = true;
		bool enableFaceCulling = true;

		bool drawWireframe = false;
		bool useMultiSampling = true;

		std::vector<SerializableVariable> getSerializableVariables() 
		{ 
			return 
			{
				{SerializableType::BOOLEAN, "Enable Depth Test", "Enable depth test for the renderer", &enableDepthTest},
				{SerializableType::BOOLEAN, "Enable Face Culling", "Enable face culling for the renderer", &enableFaceCulling},
				{SerializableType::BOOLEAN, "Draw Wireframe", "Draw wireframe for the renderer", &drawWireframe},
				{SerializableType::BOOLEAN, "Use Multi Sampling", "Use multi sampling for the renderer", &useMultiSampling}
			}; 
		};

	};
}