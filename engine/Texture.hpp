#pragma once

#include <string>

#include <GL/glew.h>

namespace engine
{
	class Texture
	{
	public:
		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		static Texture* create(const std::string& textureFilename);

		// TODO: abstract away
		GLuint textureIDOpenGL;

		std::string name;

		std::string filename;
	};
}