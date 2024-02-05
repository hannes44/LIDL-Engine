#pragma once
#include "Renderer/GraphicsAPI.hpp"

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

		void setViewport(int x, int y, int width, int height) override;

		void drawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color, glm::mat4 modelViewProjection) override;

		GraphicsAPIType getType() override;

	private:
		GraphicsAPIType type = GraphicsAPIType::OpenGL;
	};
}