#pragma once

#include "Core/Texture.hpp"

namespace engine
{
	struct OpenGLTextureData
	{
		unsigned char* data;
		int width;
		int height;
		int channels;

		~OpenGLTextureData();
	};

	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& textureFileName, bool isGameAsset = true);

		OpenGLTexture() {};

		~OpenGLTexture();

		void bind() const override;

		void unbind() const override;
	};
}