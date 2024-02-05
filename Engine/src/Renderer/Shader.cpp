#include "Shader.hpp"
#include "Renderer.hpp"
#include "Renderer/OpenGL/OpenGLShader.hpp"

namespace engine
{
	std::unique_ptr<Shader> Shader::create(const std::string& vertName, const std::string& fragName)
	{
		switch (Renderer::getInstance()->getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return std::make_unique<OpenGLShader>(vertName, fragName);
		default:
			return nullptr;
		}
	}
}

