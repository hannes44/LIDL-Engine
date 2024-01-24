#pragma once

#include "Texture.hpp"

namespace engine
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& textureFileName);

		void bind() const override;

		void unbind() const override;
	};
}