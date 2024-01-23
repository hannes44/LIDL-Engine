#pragma once
#include <string>

/* 
 * InputEvent.hpp
 * Simple container with information about a key or mouse event.
 * Created on: Oct 5, 2018
*/

namespace engine {
	class InputEvent {
    public:
        // Constructor
        InputEvent(int x, int y, int button, int key, const std::string& classification);

        // Getter functions
        int getX() const;
        int getY() const;
        int getButton() const;
        int getKey() const;
        const std::string& getClassification() const;

        // Setter functions
        void setX(int x);
        void setY(int y);
        void setButton(int button);
        void setKey(int key);
        void setClassification(const std::string& classification);

    private:
        int x;
        int y;
        int button;
        int key;
        std::string classification;

    };
}