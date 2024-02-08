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


		static void loadMeshFromOBJFile(const std::string& filename, MeshComponent* mesh, bool isGameAsset = true);

		static std::shared_ptr<MeshComponent> createMeshFromObjFile(const std::string& filename, bool isGameAsset = true);
		
		static void loadPrimativeMesh(PrimativeMeshType type, MeshComponent* mesh);

		static std::shared_ptr<MeshComponent> createPrimative(PrimativeMeshType type);

		static std::string primativeTypeToString(PrimativeMeshType type);

		static PrimativeMeshType stringToPrimativeType(const std::string& type);

		std::string getName() override { return componentName; };

		inline const static std::string componentName = "Mesh";


		PrimativeMeshType primativeType = NONE;

		std::string primativeTypeAsString = primativeTypeToString(primativeType);

		Material* getMaterial();

		void setMaterial(std::weak_ptr<Material> material);

		virtual std::vector<SerializableVariable> getSerializableVariables() 
		{ 
			return 
			{
				{SerializableType::STRING, "OBJ File", "The file path to the obj file", &objFileName},
				{SerializableType::STRING, "Primative Type", "The type of primative to create", &primativeTypeAsString},
				{SerializableType::STRING, "Material", "Id of the meshes material", &getMaterial()->uuid.id}
			}; 
		};

		// Generating the vertex array on demand
		std::shared_ptr<VertexArray> getVertexArray();

		std::string objFileName = "";


		//--------------------------------------

	private:
		// If the material is expired, use the default material
		Material defaultMaterial{};

		std::weak_ptr<Material> material;

		static std::shared_ptr<MeshComponent> createCube();

		static std::shared_ptr<MeshComponent> createSphere(int stacks = 100, int slices = 100);

		static std::shared_ptr<MeshComponent> createPlane();

		void createVertexArray();

		std::shared_ptr<VertexArray> vertexArray = nullptr;
	};
}