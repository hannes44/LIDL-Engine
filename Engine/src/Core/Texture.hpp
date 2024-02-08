#pragma once

#include <string>

#include <GL/glew.h>
#include "Selectable.hpp"
#include "Uuid.hpp"
#include "Serializer/Serializable.hpp"

namespace engine
{
	class Texture : public Selectable, public Serializable
	{
	public:
		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		static Texture* create(const std::string& textureFilename, bool isGameAsset = true);

		// Empty texture, can be used to render into
		static Texture* create();
		// TODO: abstract away
		GLuint textureIDOpenGL;

		std::string name = "Texture";

		std::string filename = "";

		std::string getName() override { return name; };

		UUID uuid{};

		UUID getUUID() override { return uuid; };

		std::vector<SerializableVariable> getSerializableVariables() override
		{
			return
			{
				{SerializableType::STRING, "name", "", &name},
				{SerializableType::STRING, "fileName", "", &filename},
				{SerializableType::STRING, "Id", "", &uuid}
			};

		};
	};
}