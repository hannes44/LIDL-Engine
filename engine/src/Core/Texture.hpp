#pragma once

#include <string>

#include <GL/glew.h>
#include "Selectable.hpp"
#include "Uuid.hpp"

namespace engine
{
	class Texture : public Selectable
	{
	public:
		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		static Texture* create(const std::string& textureFilename);

		// TODO: abstract away
		GLuint textureIDOpenGL;

		std::string name = "Texture";

		std::string filename = "";

		std::string getName() override { return name; };

		UUID uuid{};

		UUID getUUID() override { return uuid; };
	};
}