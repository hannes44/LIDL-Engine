#include "Shader.hpp"
#include "Renderer.hpp"
#include "OpenGLShader.hpp"

namespace engine
{
	Shader* Shader::create(std::string name, std::string vert, std::string frag)
	{
		switch (Renderer::getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return new OpenGLShader(name, vert, frag);
		default:
			return nullptr;
		}
	}
}

