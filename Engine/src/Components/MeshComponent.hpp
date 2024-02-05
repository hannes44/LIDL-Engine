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
		PLANE,
		NONE
	};

	class MeshComponent : public Component
	{
	public:
		MeshComponent(std::vector<Vertex> vertices = {}, std::vector<uint32_t> indices = {});

		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		static std::shared_ptr<MeshComponent> loadMeshFromOBJFile(const std::string& filename);
		
		static std::shared_ptr<MeshComponent> createPrimative(PrimativeMeshType type);

		static std::string primativeTypeToString(PrimativeMeshType type);

		static PrimativeMeshType stringToPrimativeType(const std::string& type);

		std::string getName() override { return componentName; };

		inline const static std::string componentName = "Mesh";

		Material material{};

		virtual std::vector<SerializableVariable> getSerializableVariables() 
		{ 
			return 
			{
				{SerializableType::STRING, "OBJ File", "The file path to the obj file", &objFileName},
				//{SerializableType::STRING, "Primative Type", "The type of primative to create", &primativeType},
				{SerializableType::STRING, "Material", "Id of the meshes material",&material.uuid.id}
			}; 
		};

		// Generating the vertex array on demand
		std::shared_ptr<VertexArray> getVertexArray();

		std::string objFileName = "";

		PrimativeMeshType primativeType;
		//--------------------------------------

	private:
		static std::shared_ptr<MeshComponent> createCube();

		void createVertexArray();

		std::shared_ptr<VertexArray> vertexArray = nullptr;
	};
}