#pragma once
#include <memory>
#include <vector>
#include <GL/glew.h>

namespace engine
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setData(const void* data, int size) = 0;

		static VertexBuffer* create(float* vertices, int size);
	};

	class IndexBuffer
	{
	public:

		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual int getCount() const = 0;

		static IndexBuffer* create(unsigned int* indices, int count);
	};

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
		virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) = 0;

		virtual std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const = 0;
		virtual std::shared_ptr<IndexBuffer> getIndexBuffer() const = 0;

		static VertexArray* create();
		GLuint vertexArrayId; // TODO abstract away
	};
}