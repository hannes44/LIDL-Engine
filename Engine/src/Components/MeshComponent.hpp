#pragma once

#include "Component.hpp"
#include "Components/CameraComponent.hpp"
#include "Renderer/Vertex.hpp"
#include "Core/BoundingBox.hpp"

#include <vector>
#include <string>

#include "Renderer/Buffer.hpp"

#include "Core/Material.hpp"


namespace engine
{
	enum PrimitiveMeshType
	{
		CUBE,
		SPHERE,
		CYLINDER,
		PLANE,
		NONE
	};

	// Storing the mesh data in a separate class to allow sharing between multiple meshes
	struct MeshData
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
	};

	class MeshComponent : public Component
	{
	public:
		MeshComponent(std::vector<Vertex> vertices = {}, std::vector<uint32_t> indices = {});

		static void loadMeshFromOBJFile(const std::string& filename, MeshComponent* mesh, bool isGameAsset = true);
		static void loadPrimitiveMesh(PrimitiveMeshType type, MeshComponent* mesh);

		static std::shared_ptr<MeshComponent> createMeshFromObjFile(const std::string& filename, bool isGameAsset = true);
		static std::shared_ptr<MeshComponent> createPrimitive(PrimitiveMeshType type);
		static std::shared_ptr<MeshComponent> createSphere(int stacks = 100, int slices = 100);

		static std::string primitiveTypeToString(PrimitiveMeshType type);
		static PrimitiveMeshType stringToPrimitiveType(const std::string& type);

		std::string getName() override { return componentName; };

		void drawBoundingBox(CameraComponent* camera);

		void setMaterial(std::weak_ptr<Material> material);
		std::shared_ptr<Material> getMaterial();

		BoundingBox getBoundingBox();

		// Generating the vertex array on demand
		std::shared_ptr<VertexArray> getVertexArray();

		inline const static std::string componentName = "Mesh";

		// If true, the mesh will be rendered using the camera transform * model transform
		// Useful for rendering objects that always are in front of the camera
		// For example weapons in first person shooters
		bool renderFromCameraTransform = false;

		bool isVisible = true;

		std::shared_ptr<MeshData> meshData = nullptr;

		PrimitiveMeshType primitiveType = NONE;
		std::string primitiveTypeAsString = primitiveTypeToString(primitiveType);

		std::string objFileName = "";

		std::shared_ptr<Component> clone() override {
			return std::make_shared<MeshComponent>(*this);
		}
		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::STRING, "OBJ File", "The file path to the obj file", &objFileName},
				{SerializableType::STRING, "Primitive Type", "The type of primitive to create", &primitiveTypeAsString},
				{SerializableType::STRING, "Material", "Id of the meshes material", &getMaterial()->uuid.id},
				{SerializableType::BOOLEAN, "Render From Camera Transform", "If true, the mesh will be rendered using the camera transform * model transform", &renderFromCameraTransform},
				{SerializableType::BOOLEAN, "Is Visible", "If true, the mesh will be rendered", &isVisible}
			};
		};

	private:
		static std::shared_ptr<MeshComponent> createCube();
		static std::shared_ptr<MeshComponent> createPlane();

		static glm::mat4 getComparableTransformMatrix(glm::mat4 transformMatrix);

		void createVertexArray();

		// If the material is expired, use the default material
		std::shared_ptr<Material> defaultMaterial = std::make_shared<Material>();

		std::weak_ptr<Material> material;

		glm::vec3 maxPoints{};
		void generateMaxPoints();
		glm::mat4 lastTransformMatrix{};
		bool isMaxPointsValid();

		std::shared_ptr<VertexArray> vertexArray = nullptr;
	};
}