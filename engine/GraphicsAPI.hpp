#pragma once
#include <glm/glm.hpp>
#include "Buffer.hpp"

namespace engine
{
	enum class GraphicsAPIType
	{
		OpenGL,
	};

	class GraphicsAPI
	{
	public: 
		virtual void drawIndexed(VertexArray* vertexArray, int indexCount) = 0;

		virtual void setClearColor(const glm::vec4& color) = 0;
		
		virtual void setCullFace(bool enabled) = 0;
		
		virtual void setDepthTest(bool enabled) = 0;
		
		virtual void setDrawTriangleOutline(bool enabled) = 0;

		virtual GraphicsAPIType getType() = 0;
	};
}