#pragma once
#include "InputEvent.hpp"
#include <string>

namespace engine {

    class InputListener {
    public:
        // Constructor
        InputListener();

        // Function to handle input events
        virtual void handleInput(const InputEvent& event, std::string EventType) const;

        // Getter for instance
        std::string getInstance() const;

    private:
        static int instanceCount;
        int instance;

    };

}
