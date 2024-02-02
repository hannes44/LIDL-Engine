#pragma once
#include "Uuid.hpp"

namespace engine
{
	class Selectable
	{
	public:
		virtual UUID getUUID() = 0;
		
		virtual std::string getName() = 0;
	};
}