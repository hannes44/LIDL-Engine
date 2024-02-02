#include "OpenGLGraphicsAPI.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Core/Logger.hpp"

namespace engine
{
	void OpenGLGraphicsAPI::drawIndexed(VertexArray* vertexArray, int indexCount)
	{
		if (vertexArray == nullptr)
		{
			LOG_ERROR("drawIndexed: vertexArray is null");
			return;
		}


		glBindVertexArray(vertexArray->vertexArrayId);

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLGraphicsAPI::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLGraphicsAPI::setCullFace(bool enabled)
	{
		if (enabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}
	void OpenGLGraphicsAPI::setDepthTest(bool enabled)
	{
		if (enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
	void OpenGLGraphicsAPI::setDrawTriangleOutline(bool enabled)
	{
		if (enabled)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	void OpenGLGraphicsAPI::setViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}
	GraphicsAPIType engine::OpenGLGraphicsAPI::getType()
	{
		return type;
	}
}

