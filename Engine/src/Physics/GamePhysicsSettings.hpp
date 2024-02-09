#pragma once

#include "Serializer/Serializable.hpp"

#include <string>
#include <vector>


namespace engine {
    // DO NOT USE! TODO: Use this instead of a string for the serialization of the collision type
    enum CollisionResolveType {
        DISABLED,
        FULLY_ELASTIC,
        FULLY_INELASTIC
    };

    class GamePhysicsSettings : public Serializable {
    public:
        bool enableForces = true;
        bool enableFriction = true;
        bool enableGravity = true;
        float gravity = 9.82f;

        bool enableCollisions = true;
        
        int fixedUpdateIntervalMS = 10;
		
        // TODO: Use the CollisionType when we can serialize enums
        std::string collisionResolveType = "FULLY_ELASTIC";

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
                {SerializableType::BOOLEAN, "Enable Forces", "Enable forces that generate acceleration, or only set position based on velocity", &enableForces},
                {SerializableType::BOOLEAN, "Enable Friction", "Enable friction to slow down objects", &enableFriction},
                {SerializableType::BOOLEAN, "Use Gravity", "Should gravity be used in the game", &enableGravity},
                {SerializableType::FLOAT, "Gravity", "The gravitational acceleration (g) to use", &gravity},
                {SerializableType::BOOLEAN, "Enable Collisions", "Enable collision detection and resolution", &enableCollisions},
                {SerializableType::INT, "Fixed Update Interval", "The interval in milliseconds to perform the fixed update", &fixedUpdateIntervalMS},
                {SerializableType::STRING, "Collision Type", "The type of collision to perform", &collisionResolveType}
            };
        }
    };
}
