#pragma once

#include "Component.hpp"
#include "Vertex.hpp"

#include <vector>
#include <string>

#include "Buffer.hpp"

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
	private:
		static MeshComponent createCube();
	};
}