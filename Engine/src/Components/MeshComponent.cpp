#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_relational.hpp>

#include "MeshComponent.hpp"
#include "Core/Logger.hpp"
#include <iostream>
#include "Core/ResourceManager.hpp"
#include "Utils/Utils.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../../vendor/tinyobjloader/tiny_obj_loader.h"


namespace engine
{
	MeshComponent::MeshComponent(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		meshData = std::make_shared<MeshData>(vertices, indices);

		LOG_TRACE("MeshComponent: Created mesh with {0} vertices and {1} indices", vertices.size(), indices.size());
	}

	std::shared_ptr<MeshComponent> MeshComponent::createMeshFromObjFile(const std::string& filename, bool isGameAsset)
	{
		LOG_INFO("Creating mesh from file: {0}", filename);

		std::shared_ptr<MeshComponent> mesh = std::make_shared<MeshComponent>();
		loadMeshFromOBJFile(filename, mesh.get(), isGameAsset);

		return mesh;
	}

	std::shared_ptr<Material> MeshComponent::getMaterial()
	{
		if (auto lockedMaterial = material.lock())
			return lockedMaterial;

		return defaultMaterial;
	}

	void MeshComponent::setMaterial(std::weak_ptr<Material> material)
	{
		this->material = material;
	}

	void MeshComponent::loadMeshFromOBJFile(const std::string& filename, MeshComponent* mesh, bool isGameAsset)
	{
		LOG_INFO("Loading mesh from file: {0}", filename);

		std::optional<std::shared_ptr<MeshData>> cachedMeshData = ResourceManager::getInstance()->getCachedMeshData(filename);
		if (cachedMeshData.has_value())
		{
			mesh->meshData = cachedMeshData.value();
			LOG_INFO("Loaded mesh from cache: {0}", filename);
			return;
		}

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		std::string warn;

		std::string pathToObj = isGameAsset ? ResourceManager::getInstance()->getPathToGameResource(filename) : ResourceManager::getInstance()->getPathToEditorResource(filename);
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, pathToObj.c_str())) {
			LOG_ERROR("Failed to load mesh from file: {0}", filename);
			throw std::runtime_error(err);
		}
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex{};

				if (index.vertex_index >= 0) {
					vertex.position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

				}

				if (index.normal_index >= 0) {
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}

				if (index.texcoord_index >= 0) {
					vertex.texCoords = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				// TODO: Use map to avoid duplicate vertices

				vertices.push_back(vertex);

				indices.push_back(vertices.size() - 1);
			}
		}

		mesh->objFileName = filename;
		std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>(vertices, indices);
		mesh->meshData = meshData;

		ResourceManager::getInstance()->cacheMeshData(filename, meshData);

		LOG_INFO("Loaded mesh from file: {0}", filename);
		LOG_INFO("Mesh has {0} vertices and {1} indices", vertices.size(), indices.size());
	}

	std::shared_ptr<MeshComponent> MeshComponent::createPrimitive(PrimitiveMeshType type)
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		// TODO: Implement primitive mesh creation
		switch (type)
		{
		case PrimitiveMeshType::PLANE:
			return createPlane();
		case PrimitiveMeshType::CUBE:
			return createCube();
		case PrimitiveMeshType::CYLINDER:
			break;
		case PrimitiveMeshType::SPHERE:
			return createSphere();
		default:
			LOG_ERROR("createPrimitive: {0} is not a valid primitive mesh type", type);

			return nullptr;
		}
	}

	void MeshComponent::loadPrimitiveMesh(PrimitiveMeshType type, MeshComponent* mesh)
	{
		std::shared_ptr<MeshComponent> primitive;
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		// TODO: Implement primitive mesh creation
		switch (type)
		{
		case PrimitiveMeshType::PLANE:
			primitive = createPlane();
			break;
		case PrimitiveMeshType::CUBE:
			primitive = createCube();
			break;
		case PrimitiveMeshType::CYLINDER:
			break;
		case PrimitiveMeshType::SPHERE:
			primitive = createSphere();
			break;
		default:
			LOG_ERROR("loadPrimitiveMesh: {0} is not a valid primitive mesh type", type);
		}
		if (primitive == nullptr)
			return;

		mesh->meshData = primitive->meshData;
	}

	std::string MeshComponent::primitiveTypeToString(PrimitiveMeshType type)
	{
		switch (type)
		{
		case PrimitiveMeshType::PLANE:
			return "PLANE";
		case PrimitiveMeshType::CUBE:
			return "CUBE";
		case PrimitiveMeshType::CYLINDER:
			return "CYLINDER";
		case PrimitiveMeshType::SPHERE:
			return "SPHERE";
		case PrimitiveMeshType::NONE:
			return "NONE";
		default:
			LOG_ERROR("primitiveTypeToString: {0} is not a valid primitive mesh type", type);
			return "";
		}
	}

	PrimitiveMeshType MeshComponent::stringToPrimitiveType(const std::string& type)
	{
		if (type == "PLANE")
			return PrimitiveMeshType::PLANE;
		else if (type == "CUBE")
			return PrimitiveMeshType::CUBE;
		else if (type == "CYLINDER")
			return PrimitiveMeshType::CYLINDER;
		else if (type == "SPHERE")
			return PrimitiveMeshType::SPHERE;
		else
		{
			LOG_ERROR("stringToPrimitiveType: {0} is not a valid primitive mesh type", type);
			return PrimitiveMeshType::CUBE;
		}
	}

	void MeshComponent::drawBoundingBox(CameraComponent* camera) {
		BoundingBox box = getBoundingBox();
		Utils::drawBoundingBox(box, camera, glm::vec3(0, 0.7f, 0.3f));
	}

	void MeshComponent::generateMaxPoints() {
		LOG_TRACE("Generating max points for mesh");
		Transform globalTransform = gameObject->getGlobalTransform();
		maxPoints = glm::vec3();

		for (auto& vertex : meshData->vertices) {
			glm::vec4 worldPosition = globalTransform.transformMatrix * glm::vec4(vertex.position, 0);

			maxPoints.x = std::max(maxPoints.x, std::abs(worldPosition.x));
			maxPoints.y = std::max(maxPoints.y, std::abs(worldPosition.y));
			maxPoints.z = std::max(maxPoints.z, std::abs(worldPosition.z));
		}

		lastTransformMatrix = getComparableTransformMatrix(globalTransform.transformMatrix);
	}

	// The max points are still valid if the transform matrix is translated, so we need to remove the translation data from the matrix to compare
	glm::mat4 MeshComponent::getComparableTransformMatrix(glm::mat4 transformMatrix) {
		glm::mat4 comparableMatrix = transformMatrix;
		comparableMatrix[3] = glm::vec4{ 0, 0, 0, 1 };
		return comparableMatrix;
	}

	// If the transform matrix has changed, the max points need to be recalculated
	bool MeshComponent::isMaxPointsValid() {
		return glm::all(glm::equal(lastTransformMatrix, getComparableTransformMatrix(gameObject->getGlobalTransform().transformMatrix)));
	}

	BoundingBox MeshComponent::getBoundingBox() {
		if (!isMaxPointsValid())
			generateMaxPoints();

		return BoundingBox(gameObject->getGlobalTransform().getPosition(), maxPoints * glm::vec3(2));
	}

	std::shared_ptr<VertexArray> MeshComponent::getVertexArray()
	{
		if (vertexArray == nullptr)
			createVertexArray();

		return vertexArray;

	}
	std::shared_ptr<MeshComponent> MeshComponent::createCube()
	{
		LOG_TRACE("Creating cube mesh");
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		// Front
		vertices.push_back({ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
		vertices.push_back({ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
		vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
		vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1, 1, 0), glm::vec3(0, 0, 1), glm::vec2(0, 1) });

		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(2); indices.push_back(3); indices.push_back(0);


		// Back
		vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
		vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
		vertices.push_back({ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
		vertices.push_back({ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0, 0, -1), glm::vec2(0, 1) });

		indices.push_back(6); indices.push_back(5); indices.push_back(4);
		indices.push_back(4); indices.push_back(7); indices.push_back(6);


		// Left
		vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
		vertices.push_back({ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
		vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });
		vertices.push_back({ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });

		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(10); indices.push_back(11); indices.push_back(8);

		// Right
		vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
		vertices.push_back({ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
		vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
		vertices.push_back({ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(1, 0, 0), glm::vec2(0, 1) });

		indices.push_back(14); indices.push_back(13); indices.push_back(12);
		indices.push_back(12); indices.push_back(15); indices.push_back(14);

		// Top
		vertices.push_back({ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
		vertices.push_back({ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
		vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
		vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0), glm::vec2(0, 1) });

		indices.push_back(18); indices.push_back(17); indices.push_back(16);
		indices.push_back(16); indices.push_back(19); indices.push_back(18);

		// Bottom
		vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
		vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
		vertices.push_back({ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0), glm::vec2(1, 1) });
		vertices.push_back({ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1, 1, 0), glm::vec3(0, -1, 0), glm::vec2(0, 1) });

		indices.push_back(20); indices.push_back(21); indices.push_back(22);
		indices.push_back(22); indices.push_back(23); indices.push_back(20);

		MeshComponent cube = MeshComponent(vertices, indices);
		cube.primitiveType = PrimitiveMeshType::CUBE;
		cube.primitiveTypeAsString = primitiveTypeToString(PrimitiveMeshType::CUBE);
		return std::make_shared<MeshComponent>(cube);
	}
	std::shared_ptr<MeshComponent> MeshComponent::createSphere(int stacks, int slices)
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		float radius = 1.0f;
		float sectorStep = 2 * 3.1415 / slices;
		float stackStep = 3.1415 / stacks;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stacks; ++i)
		{
			stackAngle = 3.1415 / 2 - i * stackStep;
			float xy = radius * cosf(stackAngle);
			float z = radius * sinf(stackAngle);

			for (int j = 0; j <= slices; ++j)
			{
				sectorAngle = j * sectorStep;

				float x = xy * cosf(sectorAngle);
				float y = xy * sinf(sectorAngle);

				glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
				glm::vec2 uv = glm::vec2((float)j / slices, (float)i / stacks);

				vertices.push_back({ glm::vec3(x, y, z), glm::vec3(1, 0, 0), normal, uv });
			}
		}

		int k1, k2;

		for (int i = 0; i < stacks; ++i)
		{
			k1 = i * (slices + 1);
			k2 = k1 + slices + 1;

			for (int j = 0; j < slices; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				if (i != (stacks - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		std::shared_ptr<MeshComponent> mesh = std::make_shared<MeshComponent>(vertices, indices);
		mesh->primitiveType = PrimitiveMeshType::SPHERE;
		mesh->primitiveTypeAsString = primitiveTypeToString(PrimitiveMeshType::SPHERE);
		return mesh;
	}
	std::shared_ptr<MeshComponent> MeshComponent::createPlane()
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		vertices.push_back({ glm::vec3(-5.f, 0.0f, -5.f), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
		vertices.push_back({ glm::vec3(5.f, 0.0f, -5.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
		vertices.push_back({ glm::vec3(5.f, 0.0f, 5.f), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
		vertices.push_back({ glm::vec3(-5.f, 0.0f, 5.f), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0), glm::vec2(0, 1) });

		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(2); indices.push_back(3); indices.push_back(0);
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(0); indices.push_back(3); indices.push_back(2);

		std::shared_ptr<MeshComponent> mesh = std::make_shared<MeshComponent>(vertices, indices);
		mesh->primitiveType = PrimitiveMeshType::PLANE;
		mesh->primitiveTypeAsString = primitiveTypeToString(PrimitiveMeshType::PLANE);
		return mesh;
	}
	void MeshComponent::createVertexArray()
	{
		// The vertex array creation is currently not abstracted from OpenGL
		// TODO: Abstract vertex array

		// Parsing the vertex data into a format that OpenGL expects
		std::vector<glm::vec3> positions{};
		std::vector<glm::vec3> colors{};
		std::vector<glm::vec3> normals{};
		std::vector<glm::vec2> uvs{};

		for (auto vertex : meshData->vertices)
		{
			positions.push_back(vertex.position);
			colors.push_back(vertex.color);
			normals.push_back(vertex.normal);
			uvs.push_back(vertex.texCoords);
		}


		// -----------------------------------------------------------------------
		// VERTEX ARRAY OBJECT
		// -----------------------------------------------------------------------

		vertexArray = std::shared_ptr<VertexArray>(VertexArray::create());
		vertexArray->bind();

		// -----------------------------------------------------------------------
		// POSITION BUFFER
		// -----------------------------------------------------------------------

		std::shared_ptr<VertexBuffer> positionBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::create((float*)positions.data(), positions.size() * sizeof(glm::vec3)));
		vertexArray->addVertexBuffer(positionBuffer);

		positionBuffer->bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, false /*normalized*/, 0 /*stride*/, 0 /*offset*/); // TODO: Add vertex layout to vertex buffer
		glEnableVertexAttribArray(0);

		// -----------------------------------------------------------------------
		// COLOR BUFFER
		// -----------------------------------------------------------------------

		std::shared_ptr<VertexBuffer> colorBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::create((float*)colors.data(), colors.size() * sizeof(glm::vec3)));
		vertexArray->addVertexBuffer(colorBuffer);

		colorBuffer->bind();
		glVertexAttribPointer(1, 3, GL_FLOAT, false /*normalized*/, 0 /*stride*/, 0 /*offset*/);
		glEnableVertexAttribArray(1);

		// -----------------------------------------------------------------------
		// NORMAL BUFFER
		// -----------------------------------------------------------------------

		std::shared_ptr<VertexBuffer> normalBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::create((float*)normals.data(), normals.size() * sizeof(glm::vec3)));
		vertexArray->addVertexBuffer(normalBuffer);

		normalBuffer->bind();
		glVertexAttribPointer(2, 3, GL_FLOAT, false /*normalized*/, 0 /*stride*/, 0 /*offset*/);
		glEnableVertexAttribArray(2);

		// -----------------------------------------------------------------------
		// UV BUFFER
		// -----------------------------------------------------------------------

		std::shared_ptr<VertexBuffer> uvBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::create((float*)uvs.data(), uvs.size() * sizeof(glm::vec2)));
		vertexArray->addVertexBuffer(uvBuffer);

		uvBuffer->bind();

		glVertexAttribPointer(3, 2, GL_FLOAT, false /*normalized*/, 0 /*stride*/, 0 /*offset*/);
		glEnableVertexAttribArray(3);

		// -----------------------------------------------------------------------
		// INDEX BUFFER
		// -----------------------------------------------------------------------

		std::shared_ptr<IndexBuffer> indexBuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::create(meshData->indices.data(), meshData->indices.size()));
		vertexArray->setIndexBuffer(indexBuffer);
	}
}
