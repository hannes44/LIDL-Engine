#pragma once
#include <string>

namespace engine
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;


		virtual void setInt(const std::string& name, int value) = 0;
		virtual void setFloat(const std::string& name, float value) = 0;
		virtual void setVec2(const std::string& name, float x, float y) = 0;
		virtual void setVec3(const std::string& name, float x, float y, float z) = 0;
		virtual void setVec4(const std::string& name, float x, float y, float z, float w) = 0;
		virtual void setMat4(const std::string& name, float* value) = 0;

		static Shader* create(const std::string& vertName, const std::string& fragName);
	};
}