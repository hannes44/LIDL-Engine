#include "Bootstrap.hpp"

namespace engine
{
	Bootstrap& Bootstrap::getInstance()
	{
		if (instance == nullptr)
			instance = new Bootstrap();

		return *instance;
	}

	Display* Bootstrap::getDisplay()
	{
		return display;
	}

}