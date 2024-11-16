// Player.hpp
#ifndef PLAYER_H
#define PLAYER_H

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <SDL.h>
#elif defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "Constants.hpp"
#include "SpriteSet.hpp"

class Player {
   public:
    std::string playerName;
    SpriteSet spriteSet;
    SDL_Surface* spriteSurface;
    SDL_Texture *spriteTexture;

    int direction;
    SDL_Rect position;

    int movingCooldown;
    PlayerStateEnum movingState;

    int firingCooldown;
    PlayerStateEnum firingState;

    int explodingTimer;
    PlayerStateEnum explodingState;

    Player(const std::string& playerName)
        : playerName(playerName),
          spriteSet("Jogador1")  // Initialize spriteSet here
    {
        resetStates();

        direction = static_cast<int>(DirectionEnum::RIGHT);
        spriteSet.addRect(directionToString(DirectionEnum::RIGHT), 0, 0, 32,
                          32);  // Virado para direita
        spriteSet.addRect(directionToString(DirectionEnum::UP), 34, 34, 32,
                          32);  // Virado para cima
        spriteSet.addRect(directionToString(DirectionEnum::DOWN), 34, 0, 32,
                          32);  // Virado para baixo
        spriteSet.addRect(directionToString(DirectionEnum::LEFT), 0, 34, 32,
                          32);  // Virado para esquerda
    };

    ~Player() {
        SDL_DestroyTexture(spriteTexture);
    }

    void resetStates() {
        movingState = PlayerStateEnum::IDLE;
        firingState = PlayerStateEnum::IDLE;
        explodingState = PlayerStateEnum::STABLE;
    }

    void loadSprite(const std::string& spriteFileName, SDL_Renderer *renderer) {
        spriteSurface = SDL_LoadBMP(spriteFileName.c_str());
        SDL_SetColorKey(
            spriteSurface, SDL_TRUE,
            SDL_MapRGB(spriteSurface->format, 0, 255, 0));
        spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);
    };

    void setDirection(DirectionEnum directionEnum) { direction = static_cast<int>(directionEnum); }

    void tryWalkUp() {
        if (this->movingState == PlayerStateEnum::IDLE) {
            this->position.y = this->position.y - PLAYER_MOVING_UNITS;
            this->setDirection(DirectionEnum::UP);

            this->movingState = PlayerStateEnum::MOVING;
            this->movingCooldown = PLAYER_MOVING_TIMER;
        }
    }

    void tryWalkDown() {
        if (this->movingState == PlayerStateEnum::IDLE) {
            this->position.y = this->position.y + PLAYER_MOVING_UNITS;
            this->setDirection(DirectionEnum::DOWN);

            this->movingState = PlayerStateEnum::MOVING;
            this->movingCooldown = PLAYER_MOVING_TIMER;
        }
    }

    void tryWalkRight() {
        if (this->movingState == PlayerStateEnum::IDLE) {
            this->position.x = this->position.x + PLAYER_MOVING_UNITS;
            this->setDirection(DirectionEnum::RIGHT);

            this->movingState = PlayerStateEnum::MOVING;
            this->movingCooldown = PLAYER_MOVING_TIMER;
        }
    }

    void tryWalkLeft() {
        if (this->movingState == PlayerStateEnum::IDLE) {
            this->position.x = this->position.x - PLAYER_MOVING_UNITS;
            this->setDirection(DirectionEnum::LEFT);

            this->movingState = PlayerStateEnum::MOVING;
            this->movingCooldown = PLAYER_MOVING_TIMER;
        }
    }

    void processExplodingState() {
        if (this->explodingState == PlayerStateEnum::EXPLODING) {
            if (this->explodingTimer > 0) {
                this->explodingTimer--;
            } else {
                this->explodingState = PlayerStateEnum::IDLE;
                // this->position.x = 15, this->position.y = 170;
            }
        }
    }

    void processMovingState() {
        if (this->movingState == PlayerStateEnum::MOVING) {
            if (this->movingCooldown > 0) {
                this->movingCooldown--;
            } else {
                this->movingState = PlayerStateEnum::IDLE;
            }
        }
    }

    void processFiringState() {
        if (this->firingState == PlayerStateEnum::FIRING) {
            if (this->firingCooldown > 0) {
                this->firingCooldown--;
            } else {
                this->firingState = PlayerStateEnum::IDLE;
            }
        }
    }

    void processAllStates() {
        processExplodingState();
        processMovingState();
        processFiringState();
    }

   private:
};

#endif  // PLAYER_H