#pragma once
#include "Shader.hpp"
#include <string>
#include <GL/glew.h>

namespace engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string vert, std::string frag);
		~OpenGLShader();

		void bind();
		void unbind();

		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setVec2(const std::string& name, float x, float y);
		void setVec3(const std::string& name, float x, float y, float z);
		void setVec4(const std::string& name, float x, float y, float z, float w);
		void setMat4(const std::string& name, float* value);

	private:
		int openglShaderID = 0;

		static GLuint loadShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		static std::string getShaderInfoLog(GLuint obj);
	};
}