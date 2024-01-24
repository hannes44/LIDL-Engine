#include "OpenGLBuffer.hpp"

namespace engine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, int size)
	{
		glGenBuffers(1, &openglID);
		glBindBuffer(GL_ARRAY_BUFFER, openglID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &openglID);
	}

	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, openglID);

	}

	void OpenGLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::setData(const void* data, int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, openglID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, int count) : count(count)
	{
		glGenBuffers(1, &openglBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openglBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(float), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &openglBufferID);
	}

	int OpenGLIndexBuffer::getCount() const
	{
		return count;
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openglBufferID);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &vertexArrayId);
		glBindVertexArray(vertexArrayId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &vertexArrayId);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(vertexArrayId);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		bind();
		vertexBuffer->bind();

		vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
	{
		bind();
		indexBuffer->bind();

		this->indexBuffer = indexBuffer;
	}

	std::vector<std::shared_ptr<VertexBuffer>> OpenGLVertexArray::getVertexBuffers() const
	{
		return std::vector<std::shared_ptr<VertexBuffer>>();
	}

	std::shared_ptr<IndexBuffer> OpenGLVertexArray::getIndexBuffer() const
	{
		return std::shared_ptr<IndexBuffer>();
	}
}
