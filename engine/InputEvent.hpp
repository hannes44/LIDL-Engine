#pragma once
#include <string>

namespace engine {

    class InputEvent {
    public:
        // Constructor
        InputEvent(float x, float y, float button, std::string key, const std::string& classification);

        // Getter functions
        float getX() const;
        float getY() const;
        float getButton() const;
        std::string getKey() const;
        const std::string& getClassification() const;

        // Setter functions
        void setX(float x);
        void setY(float y);
        void setButton(float button);
        void setKey(std::string key);
        void setClassification(const std::string& classification);

    private:
        float x;
        float y;
        float button;
        std::string key;
        std::string classification;
    };

}
