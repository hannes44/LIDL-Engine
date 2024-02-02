#include "MeshComponent.hpp"
#include "Core/Logger.hpp"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../vendor/tinyobjloader/tiny_obj_loader.h"

namespace engine
{
	#define MODELS_PATH "../../assets/3DObjects/"

	MeshComponent::MeshComponent(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		if (vertices.size() < 3)
			LOG_WARN("MeshComponent: vertices size is less than 3");

		if (indices.size() < 3)
			LOG_WARN("MeshComponent: indices size is less than 3");

		this->vertices = vertices;
		this->indices = indices;

		// The mesh creation is currently not abstracted from OpenGL
		// TODO: Abstract mesh creation

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
		// TODO: BUILD THE BOUNDING BOX
		// -----------------------------------------------------------------------


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

		LOG_INFO("MeshComponent: Created mesh with {0} vertices and {1} indices", vertices.size(), indices.size());
	}

	std::shared_ptr<MeshComponent> MeshComponent::loadMeshFromOBJFile(const std::string& filename)
	{
		LOG_INFO("Loading mesh from file: {0}", filename);

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		std::string warn;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (MODELS_PATH + filename).c_str())) {
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
		MeshComponent mesh = MeshComponent(vertices, indices);
		mesh.objFileName = filename;

		LOG_INFO("Created mesh from file: {0}", filename);

		return std::make_shared<MeshComponent>(mesh);
	}

	std::shared_ptr<MeshComponent> MeshComponent::createPrimative(PrimativeMeshType type)
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		// TODO: Implement primative mesh creation
		switch (type)
		{
		case PrimativeMeshType::PLANE:
			break;
		case PrimativeMeshType::CUBE:
			return createCube();
		case PrimativeMeshType::CYLINDER:
			break;
		case PrimativeMeshType::SPHERE:
			break;
		default:
			LOG_ERROR("PrimativeMeshType: {0} is not a valid primative mesh type", type);

			return nullptr;
		}
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
		default:
			LOG_ERROR("PrimativeMeshType: {0} is not a valid primative mesh type", type);
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
			LOG_ERROR("PrimativeMeshType: {0} is not a valid primative mesh type", type);
			return PrimativeMeshType::CUBE;
		}
	}
	std::shared_ptr<MeshComponent> MeshComponent::createCube()
	{
		LOG_INFO("Creating cube mesh");
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
		return std::make_shared<MeshComponent>(cube);
	}
}
