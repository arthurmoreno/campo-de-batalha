#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

enum struct DirectionEnum { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };

enum struct PlayerStateEnum { IDLE = 0, MOVING = 1, FIRING = 2, STABLE = 3, EXPLODING = 4 };

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

const int PROJECTILE_MOVING_COOLDOWN = 0;

const int PROJECTILE_MOVING_UNITS = 6;

const int PLAYER_COLLISION_BOX_SHRINK = 2;

const int PLAYER_EXPLODING_TIMER = 25;

const int PLAYER_MOVING_UNITS = 6;

const int PLAYER_MOVING_TIMER = 1;

const int PLAYER_FIRING_TIMER = 5;

#endif  // CONSTANTS_H