#pragma once
#include "InputSystem.hpp"

namespace engine
{
	class InputFramework : public InputSystem
	{
    private:
        double tick, timeInterval;

    public:
        // Constructor
        InputFramework();

        // Override functions from InputSystem
        void getInput() override;
        void initialize() override;

	};
}