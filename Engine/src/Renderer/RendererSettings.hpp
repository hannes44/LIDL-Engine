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

		bool enableFog = false;
		// Linear fog distance
		float startFogDistance = 0.0f;
		float endFogDistance = 100.0f;

		bool drawColliders = false;
		bool drawColliderAABBs = false;
		bool drawMeshBoundingBoxes = false;

		// The width and height to render into
		int width = 1280;
		int height = 720;

		glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 1.0f);

		std::vector<SerializableVariable> getSerializableVariables()
		{
			return
			{
				{SerializableType::BOOLEAN, "Enable Depth Test", "Enable depth test for the renderer", &enableDepthTest},
				{SerializableType::BOOLEAN, "Enable Face Culling", "Enable face culling for the renderer", &enableFaceCulling},
				{SerializableType::BOOLEAN, "Draw Wireframe", "Draw wireframe for the renderer", &drawWireframe},
				{SerializableType::BOOLEAN, "Use Multi Sampling", "Use multi sampling for the renderer", &useMultiSampling},
				{SerializableType::BOOLEAN, "Draw Colliders", "Draw all colliders", &drawColliders},
				{SerializableType::BOOLEAN, "Draw Collider AABBs", "Draw axis aligned bounding boxes for all colliders", &drawColliderAABBs},
				{SerializableType::BOOLEAN, "Draw Mesh Bounding Boxes", "Draw bounding boxes for all meshes", &drawMeshBoundingBoxes},
				{SerializableType::COLOR, "Background Color", "The background color of the renderer", &backgroundColor},
				{SerializableType::BOOLEAN, "Enable Fog", "Enable fog for the renderer", &enableFog},
				{SerializableType::FLOAT, "Start Fog Distance", "The start distance of the fog", &startFogDistance},
				{SerializableType::FLOAT, "End Fog Distance", "The end distance of the fog", &endFogDistance},
			};
		};

	};
}