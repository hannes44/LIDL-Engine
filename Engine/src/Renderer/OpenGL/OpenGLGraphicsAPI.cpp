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

	// Performance heavy since new shaders, vao , vbo, and ebo are created every time. Only use for debug purposes
	void OpenGLGraphicsAPI::drawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color, glm::mat4 modelViewProjection)
	{
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"uniform mat4 MVP;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec3 color;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(color, 1.0f);\n"
			"}\n\0";

		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// link shaders
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::vector<float> vertices = {
			 start.x, start.y, start.z,
			 end.x, end.y, end.z
		};

		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(shaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &modelViewProjection[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
	}
	GraphicsAPIType engine::OpenGLGraphicsAPI::getType()
	{
		return type;
	}
}

