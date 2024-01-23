#include "Bootstrap.hpp"
#include "chrono"

namespace engine
{
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	static TimePoint previousTime = Clock::now();

	double Bootstrap::getDeltaTime() {
		TimePoint currentTime = Clock::now();
		std::chrono::duration<double> deltaTime = currentTime - previousTime;

		previousTime = currentTime;

		return deltaTime.count();
	}

	Bootstrap& Bootstrap::getInstance()
	{
		if (instance == nullptr)
			instance = new Bootstrap();

		return *instance;
	}

	Display& Bootstrap::getDisplay()
	{
		return *display;
	}

}