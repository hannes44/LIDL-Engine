#pragma once
#include <string>
#include <random>
#include <functional>

namespace engine
{
	class UUID
	{
	public:
		UUID();

		// Custom equality operator
		bool operator==(const UUID& other) const
		{
			return id == other.id;
		}

		// Custom inequality operator
		bool operator!=(const UUID& other) const
		{
			return id != other.id;
		}

		std::string id = "";
	};
}