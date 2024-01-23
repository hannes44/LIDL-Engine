#include "Buffer.hpp"
#include "Renderer.hpp"
#include "OpenGLBuffer.hpp"
#include "Logger.hpp"

namespace engine
{
	VertexBuffer* VertexBuffer::create(float* vertices, int size)
	{
		switch (Renderer::getAPIType())
		{
			case GraphicsAPIType::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
			default:
				LOG_FATAL("Unknown Graphics API");
				return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::create(unsigned int* indices, int count)
	{
		switch (Renderer::getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		default:
			LOG_FATAL("Unknown Graphics API");
			return nullptr;
		}
	}

	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return new OpenGLVertexArray();
		default:
			LOG_FATAL("Unknown Graphics API");
			return nullptr;
		}
	}

}
