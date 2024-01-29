#pragma once

#include "Component.hpp"
#include "Vertex.hpp"

#include <vector>
#include <string>

#include "Buffer.hpp"

#include "Material.hpp"

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

		std::shared_ptr<VertexArray> vertexArray = nullptr;

		static MeshComponent loadMeshFromOBJFile(const std::string& filename);
		
		static MeshComponent createPrimative(PrimativeMeshType type);

		static std::string primativeTypeToString(PrimativeMeshType type);

		static PrimativeMeshType stringToPrimativeType(const std::string& type);

		std::string getName() override { return "Mesh"; };

		
		// -Serialized properties-
		Material material{};

		std::string objFileName = "";

		PrimativeMeshType primativeType;
		//--------------------------------------

	private:
		static MeshComponent createCube();
	};
}