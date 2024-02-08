#include "Texture.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/OpenGL/OpenGLTexture.hpp"


namespace engine
{
	Texture* Texture::create(const std::string& textureFilename, bool isGameAsset)
	{
		switch (Renderer::getInstance()->getAPIType())
		{
		case GraphicsAPIType::OpenGL:
			return new OpenGLTexture(textureFilename, isGameAsset);
			break;
		default:
			break;
		}
	}
}
