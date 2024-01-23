#pragma once
#include "GraphicsAPI.hpp"

namespace engine
{
	class OpenGLGraphicsAPI : public GraphicsAPI
	{
	public:
		void drawIndexed(VertexArray* vertexArray, int indexCount);

		void setClearColor(const glm::vec4& color);

		void setCullFace(bool enabled);

		void setDepthTest(bool enabled);

		void setDrawTriangleOutline(bool enabled);

		virtual GraphicsAPIType getType();

	private:
		GraphicsAPIType type = GraphicsAPIType::OpenGL;
	};
}