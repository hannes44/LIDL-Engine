#pragma once
#include "InputEvent.hpp"
#include <string>

namespace engine
{
	class InputListener
	{
    public:
        // Constructor
        InputListener(std::string instance);

        // Getter for instance
        std::string getInstance() const;

        // Function to handle input events
        void handleInput(const InputEvent& event) const;

    private:
        std::string instance;

	};
}