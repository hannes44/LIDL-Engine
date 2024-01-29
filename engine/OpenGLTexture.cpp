#include "OpenGLTexture.hpp"
#include "Logger.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"

namespace engine
{
	#define PATH_TO_TEXTURES "../../assets/textures/"

	OpenGLTexture::OpenGLTexture(const std::string& textureFilename)
	{
		LOG_INFO("Creating texture " + textureFilename);
		filename = textureFilename;

		std::string filenameNoExtension = textureFilename.substr(0, textureFilename.find_last_of("."));
		name = filenameNoExtension;

		glGenTextures(1, &textureIDOpenGL);
		glBindTexture(GL_TEXTURE_2D, textureIDOpenGL);

		// Load and generate the texture
		int width, height, nrChannels;
		unsigned char* data = stbi_load((PATH_TO_TEXTURES + textureFilename).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{

			// Generate the texture
			if (nrChannels == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
					GL_UNSIGNED_BYTE, data);
			}
			else if (nrChannels == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, data);
			}
			else
			{
				LOG_ERROR("Texture has an unsupported number of channels");
				throw std::runtime_error("Texture has an unsupported number of channels");
			}

			// Generate the mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			LOG_ERROR("Failed to load texture " + textureFilename);
			throw std::runtime_error("Failed to load texture");
		}
		stbi_image_free(data);

		LOG_INFO("Texture " + textureFilename + " created");
	}
	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &textureIDOpenGL);
	}

	void OpenGLTexture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, textureIDOpenGL);
	}
	void OpenGLTexture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

