#include "OpenGLShader.hpp"
#include "Logger.hpp"
#include <GL/glew.h>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace engine
{
	OpenGLShader::OpenGLShader(std::string name, std::string vertPath, std::string fragPath)
	{
		openglShaderID = loadShaderProgram(vertPath, fragPath);
	}

	OpenGLShader::~OpenGLShader()
	{
		LOG_INFO("Deleting openGL shader program: %d", openglShaderID);
		glDeleteProgram(openglShaderID);
	}

	void OpenGLShader::bind()
	{
		glUseProgram(openglShaderID);
	}

	void OpenGLShader::unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::setInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(openglShaderID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::setFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(openglShaderID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::setVec2(const std::string& name, float x, float y)
	{
		GLint location = glGetUniformLocation(openglShaderID, name.c_str());
		glUniform2f(location, x, y);
	}

	void OpenGLShader::setVec3(const std::string& name, float x, float y, float z)
	{
		GLint location = glGetUniformLocation(openglShaderID, name.c_str());
		glUniform3f(location, x, y, z);
	}

	void OpenGLShader::setVec4(const std::string& name, float x, float y, float z, float w)
	{
		GLint location = glGetUniformLocation(openglShaderID, name.c_str());
		glUniform4f(location, x, y, z, w);
	}

	void OpenGLShader::setMat4(const std::string& name, float* value)
	{
		GLint location = glGetUniformLocation(openglShaderID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	}

	GLuint OpenGLShader::loadShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

		std::ifstream vs_file(vertexShader);
		std::string vs_src((std::istreambuf_iterator<char>(vs_file)), std::istreambuf_iterator<char>());

		std::ifstream fs_file(fragmentShader);
		std::string fs_src((std::istreambuf_iterator<char>(fs_file)), std::istreambuf_iterator<char>());

		const char* vs = vs_src.c_str();
		const char* fs = fs_src.c_str();

		glShaderSource(vShader, 1, &vs, nullptr);
		glShaderSource(fShader, 1, &fs, nullptr);
		// text data is not needed beyond this point

		glCompileShader(vShader);
		int compileOk = 0;
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileOk);
		if (!compileOk)
		{
			std::string err = getShaderInfoLog(vShader);
			std::cout << vertexShader << " error: " << err << std::endl;
			throw std::runtime_error(err);
			return 0;
		}

		glCompileShader(fShader);
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileOk);
		if (!compileOk)
		{
			std::string err = getShaderInfoLog(fShader);
			std::cout << fragmentShader << " error " << err << std::endl;
			throw std::runtime_error(err);
			return 0;
		}

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, fShader);
		glDeleteShader(fShader);
		glAttachShader(shaderProgram, vShader);
		glDeleteShader(vShader);

		glLinkProgram(shaderProgram);
		GLint linkOk = 0;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkOk);

		if (!linkOk)
		{
			return 0;
		}

		return shaderProgram;
	}

	std::string OpenGLShader::getShaderInfoLog(GLuint obj)
	{
		int logLength = 0;
		int charsWritten = 0;
		char* tmpLog;
		std::string log;

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			tmpLog = new char[logLength];
			glGetShaderInfoLog(obj, logLength, &charsWritten, tmpLog);
			log = tmpLog;
			delete[] tmpLog;
		}

		return log;
	}
}
