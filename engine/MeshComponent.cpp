#include "MeshComponent.hpp"
#include "Logger.hpp"
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

		vertices = vertices;
		indices = indices;

		// The mesh creation is currently not abstracted from OpenGL
		// TODO: Abstract mesh creation

		// Parsing the vertex data into a format that OpenGL expects
		// Vulkan can use the vertex data directly
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

	MeshComponent MeshComponent::loadMeshFromOBJFile(const std::string& filename)
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

		LOG_INFO("Created mesh from file: {0}", filename);

		return mesh;
	}

	MeshComponent MeshComponent::createPrimative(PrimativeMeshType type)
	{
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		// TODO: Implement primative mesh creation
		switch (type)
		{
		case PrimativeMeshType::PLANE:
			break;
		case PrimativeMeshType::CUBE:
			break;
		case PrimativeMeshType::CYLINDER:
			break;
		case PrimativeMeshType::SPHERE:
			break;
		default:
			LOG_ERROR("PrimativeMeshType: {0} is not a valid primative mesh type", type);

			return MeshComponent(vertices, indices);
		}
	}
}
