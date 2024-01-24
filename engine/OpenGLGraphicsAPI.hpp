#pragma once
#include "GraphicsAPI.hpp"

namespace engine
{
	class OpenGLGraphicsAPI : public GraphicsAPI
	{
	public:
		void drawIndexed(VertexArray* vertexArray, int indexCount) override;

		void setClearColor(const glm::vec4& color) override;

		void setCullFace(bool enabled) override;

		void setDepthTest(bool enabled) override;

		void setDrawTriangleOutline(bool enabled) override;

		GraphicsAPIType getType() override;

	private:
		GraphicsAPIType type = GraphicsAPIType::OpenGL;
	};
}