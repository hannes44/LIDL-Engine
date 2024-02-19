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

		bool drawBoundingBoxes = false;

		glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 1.0f);

		std::vector<SerializableVariable> getSerializableVariables()
		{
			return
			{
				{SerializableType::BOOLEAN, "Enable Depth Test", "Enable depth test for the renderer", &enableDepthTest},
				{SerializableType::BOOLEAN, "Enable Face Culling", "Enable face culling for the renderer", &enableFaceCulling},
				{SerializableType::BOOLEAN, "Draw Wireframe", "Draw wireframe for the renderer", &drawWireframe},
				{SerializableType::BOOLEAN, "Use Multi Sampling", "Use multi sampling for the renderer", &useMultiSampling},
				{SerializableType::BOOLEAN, "Draw Bounding Boxes", "Draw bounding boxes for all colliders", &drawBoundingBoxes},
				{SerializableType::COLOR, "Background Color", "The background color of the renderer", &backgroundColor}
			};
		};

	};
}