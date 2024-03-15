#include "OpenGLTexture.hpp"
#include "Core/Logger.hpp"
#include "Core/ResourceManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb/stb_image.h"

namespace engine
{
	OpenGLTexture::OpenGLTexture(const std::string& textureFilename, bool isGameAsset)
	{
		LOG_INFO("Creating texture " + textureFilename);
		filename = textureFilename;

		std::string filenameNoExtension = textureFilename.substr(0, textureFilename.find_last_of("."));
		name = filenameNoExtension;

		std::string pathToFile = isGameAsset ? ResourceManager::getInstance()->getPathToGameResource(textureFilename) : ResourceManager::getPathToEditorResource(textureFilename);

		glGenTextures(1, &textureIDOpenGL);
		glBindTexture(GL_TEXTURE_2D, textureIDOpenGL);

		unsigned char* data = nullptr;

		// Load and generate the texture
		int width, height, nrChannels;

		std::optional<std::shared_ptr<OpenGLTextureData>> textureData = ResourceManager::getInstance()->getCachedTextureData(textureFilename);

		bool isCached = textureData.has_value();
		if (isCached)
		{
			data = textureData.value()->data;
			width = textureData.value()->width;
			height = textureData.value()->height;
			nrChannels = textureData.value()->channels;
		}

		if (data == nullptr)
		{
			data = stbi_load(pathToFile.c_str(), &width, &height, &nrChannels, 0);
		}

		if (data)
		{
			if (!isCached)
			{
				// Cache the texture data
				std::shared_ptr<OpenGLTextureData> textureData = std::make_shared<OpenGLTextureData>();
				textureData->data = data;
				textureData->width = width;
				textureData->height = height;
				textureData->channels = nrChannels;
				ResourceManager::getInstance()->cacheTextureData(textureFilename, textureData);
			}

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

	OpenGLTextureData::~OpenGLTextureData()
	{
		stbi_image_free(data);
	}
}

