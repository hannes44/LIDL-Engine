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
		~OpenGLVertexBuffer() override;


		void bind() const override;
		void unbind() const override;
		void setData(const void* data, int size) override;
	private:
		GLuint openglID = 0; 
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* indices, int count);
		~OpenGLIndexBuffer() override;
		int getCount() const override;

		void bind() const override;
		void unbind() const override;
	private:
		GLuint openglBufferID;
		int count;
	};

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;
		void bind() const override;
		void unbind() const override;
		void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
		void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

		std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const override;
		std::shared_ptr<IndexBuffer> getIndexBuffer() const override;
	private:
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};
}
