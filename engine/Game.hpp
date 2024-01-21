#pragma once

namespace engine
{
	class Game
	{
	public:
		// Called every frame
		virtual void update() = 0;

		// Called at initialization
		virtual void initialize() = 0;

		bool running = true;

		const void gameLoop();

    protected:
        // 0 is uncapped
        virtual double getTargetFrameRate() = 0;

        int frameCount = 0;
        long long lastFrameTime = 0;

        static long long getTime();
	};
}