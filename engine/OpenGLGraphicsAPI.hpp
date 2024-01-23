#pragma once
#include "GraphicsAPI.hpp"

namespace engine
{
	class OpenGLGraphicsAPI : public GraphicsAPI
	{
		void drawIndexed();

		void setClearColor();

		void setCullFace();

		void setDepthTest();

		void setDrawTriangleOutline();
	};
}