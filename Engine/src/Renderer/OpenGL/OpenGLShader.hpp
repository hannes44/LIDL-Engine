#pragma once
#include "Renderer/Shader.hpp"
#include <string>
#include <GL/glew.h>

namespace engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertName, const std::string& fragNameg);
		~OpenGLShader() override;

		void bind() override;
		void unbind() override;

		void setInt(const std::string& name, int value) override;
		void setFloat(const std::string& name, float value) override;
		void setVec2(const std::string& name, float x, float y) override;
		void setVec3(const std::string& name, float x, float y, float z) override;
		void setVec4(const std::string& name, float x, float y, float z, float w) override;
		void setMat4(const std::string& name, float* value) override;

	private:
		int openglShaderID = 0;

		static GLuint loadShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		static std::string getShaderInfoLog(GLuint obj);
	};
}