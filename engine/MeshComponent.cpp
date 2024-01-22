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

		LOG_INFO("MeshComponent: Created mesh with {0} vertices and {1} indices", vertices.size(), indices.size());
	}

	MeshComponent MeshComponent::loadMeshFromOBJFile(const std::string& filename)
	{
		LOG_INFO("Loading mesh from file: {0}", filename);

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (MODELS_PATH + filename).c_str())) {
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
