#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

enum struct DirectionEnum { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };

enum struct PlayerStateEnum { IDLE = 0, MOVING = 1, FIRING = 2, EXPLODING = 3 };

// Function to convert Jogador2Direction to string
inline std::string directionToString(DirectionEnum dir) {
    switch (dir) {
        case DirectionEnum::RIGHT:
            return "RIGHT";
        case DirectionEnum::UP:
            return "UP";
        case DirectionEnum::DOWN:
            return "DOWN";
        case DirectionEnum::LEFT:
            return "LEFT";
        default:
            return "UNKNOWN";
    }
}

const int PROJECTILE_MOVING_COOLDOWN = 5;

const int PLAYER_MOVING_TIMER = 50;

const int PLAYER_FIRING_TIMER = 100;

#endif  // CONSTANTS_H