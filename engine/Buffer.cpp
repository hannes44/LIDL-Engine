#include "Buffer.hpp"
#include "Renderer.hpp"
#include "OpenGLBuffer.hpp"
#include "Logger.hpp"

namespace engine
{
	std::unique_ptr<VertexBuffer> VertexBuffer::create(float* vertices, int size)
	{
		switch (Renderer::getAPIType())
		{
			case GraphicsAPIType::OpenGL:
				return std::make_unique<OpenGLVertexBuffer>(vertices, size);
			default:
				LOG_FATAL("ERROR creating vertex buffer, Unknown Graphics API");
				abort();
		}

		return nullptr;
	}

	std::unique_ptr<IndexBuffer> IndexBuffer::create(unsigned int* indices, int count)
	{
		switch (Renderer::getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return std::make_unique<OpenGLIndexBuffer>(indices, count);
		default:
			LOG_FATAL("ERROR creating index buffer, Unknown Graphics API");
			abort();
		}

		return nullptr;
	}

	std::unique_ptr<VertexArray> VertexArray::create()
	{
		switch (Renderer::getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return std::make_unique<OpenGLVertexArray>();
		default:
			LOG_FATAL("ERROR creating vertex array, Unknown Graphics API");
			abort();
		}

		return nullptr;
	}

}
