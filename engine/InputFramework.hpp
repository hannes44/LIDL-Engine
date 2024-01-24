#pragma once
#include "InputSystem.hpp"
#include "InputEvent.hpp"

namespace engine {

    class InputFramework : public InputSystem {
    private:
        double tick;
        double timeInterval;

        // Private constructor to prevent external instantiation
        InputFramework();

    public:
        // Non cloneable
        InputFramework(InputFramework& other) = delete;

        // Non copyable
        void operator=(const InputFramework&) = delete;

        static InputFramework& getInstance();

        // Override function from InputSystem
        void getInput() override;

        // Function to initialize the input framework
        void initialize();
    };

}