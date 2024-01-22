#pragma once

#include <string>
#include <SDL.h>

namespace engine
{
    class Window
    {
    public:
        Window(Window const&) = delete;
        ~Window() {};

        static Window& getInstance();

        void createWindow(int width, int height, const std::string title);

        void cleanup();

        void getWindowSize(int* width, int* height);
        void setWindowTitle(const std::string title);

        void closeWindow();

        void newFrame();

    protected:
        inline static Window* instance = nullptr;
        Window() {};

        SDL_Window* window = nullptr;
    };
} // namespace engine