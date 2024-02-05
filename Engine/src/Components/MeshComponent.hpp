#pragma once

#include "Component.hpp"
#include "Renderer/Vertex.hpp"

#include <vector>
#include <string>

#include "Renderer/Buffer.hpp"

#include "Core/Material.hpp"

namespace engine
{
	enum PrimativeMeshType
	{
		CUBE,
		SPHERE,
		CYLINDER,
		PLANE
	};

	class MeshComponent : public Component
	{
	public:
		MeshComponent(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		static std::shared_ptr<MeshComponent> loadMeshFromOBJFile(const std::string& filename);
		
		static std::shared_ptr<MeshComponent> createPrimative(PrimativeMeshType type);

		static std::string primativeTypeToString(PrimativeMeshType type);

		static PrimativeMeshType stringToPrimativeType(const std::string& type);

		std::string getName() override { return componentName; };

		inline const static std::string componentName = "Mesh";

		// Generating the vertex array on demand
		std::shared_ptr<VertexArray> getVertexArray();

		// -Serialized properties-
		Material material{};

		std::string objFileName = "";

		PrimativeMeshType primativeType;
		//--------------------------------------

	private:
		static std::shared_ptr<MeshComponent> createCube();

		void createVertexArray();

		std::shared_ptr<VertexArray> vertexArray = nullptr;
	};
}