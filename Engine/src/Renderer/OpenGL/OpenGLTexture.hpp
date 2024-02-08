#pragma once

#include "Core/Texture.hpp"

namespace engine
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& textureFileName, bool isGameAsset = true);

		~OpenGLTexture();

		void bind() const override;

		void unbind() const override;
	};
}