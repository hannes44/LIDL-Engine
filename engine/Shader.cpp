#include "Shader.hpp"
#include "Renderer.hpp"
#include "OpenGLShader.hpp"

namespace engine
{
	Shader* Shader::create(const std::string& vertName, const std::string& fragName)
	{
		switch (Renderer::getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return new OpenGLShader(vertName, fragName);
		default:
			return nullptr;
		}
	}
}

