#include "Texture.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/OpenGL/OpenGLTexture.hpp"


namespace engine
{
	Texture* Texture::create(const std::string& textureFilename)
	{
		switch (Renderer::getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return new OpenGLTexture(textureFilename);
			break;
		default:
			break;
		}
	}
}
