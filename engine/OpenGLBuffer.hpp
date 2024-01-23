#pragma once
#include <GL/glew.h>
#include "Buffer.hpp"
#include <memory>

namespace engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, int size);
		~OpenGLVertexBuffer();


		void bind() const;
		void unbind() const;
		void setData(const void* data, int size);
	private:
		GLuint openglID = 0; 
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* indices, int count);
		~OpenGLIndexBuffer();
		int getCount() const;

		void bind() const;
		void unbind() const;
	private:
		GLuint openglBufferID;
		int count;
	};

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		void bind() const;
		void unbind() const;
		void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
		void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

		std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const;
		std::shared_ptr<IndexBuffer> getIndexBuffer() const;
	private:
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};
}
