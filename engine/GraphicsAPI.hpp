#pragma once

namespace engine
{
	class GraphicsAPI
	{
	public: 
		virtual void drawIndexed() = 0;

		virtual void setClearColor() = 0;
		
		virtual void setCullFace() = 0;
		
		virtual void setDepthTest() = 0;
		
		virtual void setDrawTriangleOutline() = 0;
	};
}