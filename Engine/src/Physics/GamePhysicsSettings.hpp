#pragma once

#include "Serializer/Serializable.hpp"

#include <string>
#include <vector>


namespace engine {
    // DO NOT USE! TODO: Use this instead of a string for the serialization of the collision type
    enum CollisionType {
        FULLY_ELASTIC,
        FULLY_INELASTIC
    };

    class GamePhysicsSettings : public Serializable {
    public:
        bool useGravity = true;
        float gravity = 9.82f;
        int fixedUpdateIntervalMS = 10;
        // TODO: Use the CollisionType when we can serialize enums
        std::string collisionType = "FULLY_ELASTIC";

        /** 
         * The scale of the fixed update to correctly scale the physics.
         * If the fixed update is every second, the scale will be 1.
         * If the fixed update is twice a second, the scale will be 0.5 since a cube with the velocity 1 unit per second should only move 0.5 units each update.
        */
        float getFixedUpdateScale() {
            return fixedUpdateIntervalMS / 1000.0f;
        }

        std::vector<SerializableVariable> getSerializableVariables() override {
            return {
                {SerializableType::BOOLEAN, "Use Gravity", "Should gravity be used in the game", &useGravity},
                {SerializableType::FLOAT, "Gravity", "The gravitational acceleration (g) to use", &gravity},
                {SerializableType::INT, "Fixed Update Interval", "The interval in milliseconds to perform the fixed update", &fixedUpdateIntervalMS},
                {SerializableType::STRING, "Collision Type", "The type of collision to perform", &collisionType}
            };
        }
    };
}
