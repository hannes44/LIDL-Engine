#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>

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

		this->vertices = vertices;
		this->indices = indices;

		// -----------------------------------------------------------------------
		// TODO: BUILD THE BOUNDING BOX
		// -----------------------------------------------------------------------




		LOG_TRACE("MeshComponent: Created mesh with {0} vertices and {1} indices", vertices.size(), indices.size());
	}

	std::shared_ptr<MeshComponent> MeshComponent::createMeshFromObjFile(const std::string& filename, bool isGameAsset)
	{
		LOG_INFO("Creating mesh from file: {0}", filename);

		std::shared_ptr<MeshComponent> mesh = std::make_shared<MeshComponent>();
		loadMeshFromOBJFile(filename, mesh.get(), isGameAsset);

		return mesh;
	}

	Material* MeshComponent::getMaterial()
	{
		if (auto lockedMaterial = material.lock())
		{
			return lockedMaterial.get();
		}
		else
		{
			return &defaultMaterial;
		}
	}

	void MeshComponent::setMaterial(std::weak_ptr<Material> material)
	{
		this->material = material;
	}

	void MeshComponent::loadMeshFromOBJFile(const std::string& filename, MeshComponent* mesh, bool isGameAsset)
	{
		LOG_INFO("Loading mesh from file: {0}", filename);

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

		mesh->vertices = vertices;
		mesh->indices = indices;
		mesh->objFileName = filename;

		LOG_INFO("Loaded mesh from file: {0}", filename);
		LOG_INFO("Mesh has {0} vertices and {1} indices", vertices.size(), indices.size());
	}

	std::shared_ptr<MeshComponent> MeshComponent::createPrimative(PrimativeMeshType type)
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		// TODO: Implement primative mesh creation
		switch (type)
		{
		case PrimativeMeshType::PLANE:
			return createPlane();
		case PrimativeMeshType::CUBE:
			return createCube();
		case PrimativeMeshType::CYLINDER:
			break;
		case PrimativeMeshType::SPHERE:
			return createSphere();
		default:
			LOG_ERROR("createPrimative: {0} is not a valid primative mesh type", type);

			return nullptr;
		}
	}

	void MeshComponent::loadPrimativeMesh(PrimativeMeshType type, MeshComponent* mesh)
	{
		std::shared_ptr<MeshComponent> primative;
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		// TODO: Implement primative mesh creation
		switch (type)
		{
		case PrimativeMeshType::PLANE:
			primative = createPlane();
			break;
		case PrimativeMeshType::CUBE:
			primative = createCube();
			break;
		case PrimativeMeshType::CYLINDER:
			break;
		case PrimativeMeshType::SPHERE:
			primative = createSphere();
			break;
		default:
			LOG_ERROR("loadPrimativeMesh: {0} is not a valid primative mesh type", type);

		}
		if (primative == nullptr)
		{
			return;
		}
		mesh->vertices = primative->vertices;
		mesh->indices = primative->indices;
	}

	std::string MeshComponent::primativeTypeToString(PrimativeMeshType type)
	{
		switch (type)
		{
		case PrimativeMeshType::PLANE:
			return "PLANE";
		case PrimativeMeshType::CUBE:
			return "CUBE";
		case PrimativeMeshType::CYLINDER:
			return "CYLINDER";
		case PrimativeMeshType::SPHERE:
			return "SPHERE";
		case PrimativeMeshType::NONE:
			return "NONE";
		default:
			LOG_ERROR("primativeTypeToString: {0} is not a valid primative mesh type", type);
			return "";
		}
	}

	PrimativeMeshType MeshComponent::stringToPrimativeType(const std::string& type)
	{
		if (type == "PLANE")
			return PrimativeMeshType::PLANE;
		else if (type == "CUBE")
			return PrimativeMeshType::CUBE;
		else if (type == "CYLINDER")
			return PrimativeMeshType::CYLINDER;
		else if (type == "SPHERE")
			return PrimativeMeshType::SPHERE;
		else
		{
			LOG_ERROR("stringToPrimativeType: {0} is not a valid primative mesh type", type);
			return PrimativeMeshType::CUBE;
		}
	}

	void MeshComponent::drawBoundingBox(CameraComponent* camera) {
		BoundingBox box = getBoundingBox();
		Utils::drawBoundingBox(box, camera, glm::vec3(0, 0.7f, 0.3f));
	}

	BoundingBox MeshComponent::getBoundingBox() {
		std::vector<glm::vec3> vertexPositions = {};

		for (auto& vertex : vertices)
			vertexPositions.push_back(vertex.position);

		const glm::quat rotation = gameObject->transform.getRotation();
		glm::vec3 maxPoints = glm::vec3();

		for (auto& vertexPosition : vertexPositions) {
			glm::vec3 rotatedPos = glm::rotate(rotation, vertexPosition);

			if (rotatedPos.x > maxPoints.x)
				maxPoints.x = rotatedPos.x;

			if (rotatedPos.y > maxPoints.y)
				maxPoints.y = rotatedPos.y;

			if (rotatedPos.z > maxPoints.z)
				maxPoints.z = rotatedPos.z;
		}

		maxPoints = glm::vec3(maxPoints.x * 2, maxPoints.y * 2, maxPoints.z * 2);

		return BoundingBox(gameObject->transform.getPosition(), maxPoints);
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
		cube.primativeType = PrimativeMeshType::CUBE;
		cube.primativeTypeAsString = primativeTypeToString(PrimativeMeshType::CUBE);
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
		mesh->primativeType = PrimativeMeshType::SPHERE;
		mesh->primativeTypeAsString = primativeTypeToString(PrimativeMeshType::SPHERE);
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
		mesh->primativeType = PrimativeMeshType::PLANE;
		mesh->primativeTypeAsString = primativeTypeToString(PrimativeMeshType::PLANE);
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

		for (auto vertex : vertices)
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

		std::shared_ptr<IndexBuffer> indexBuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::create(indices.data(), indices.size()));
		vertexArray->setIndexBuffer(indexBuffer);
	}
}
