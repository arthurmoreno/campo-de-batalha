// Player.hpp
#ifndef PLAYER_H
#define PLAYER_H

#if defined(__APPLE__)
#include <SDL/SDL.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <SDL.h>
#elif defined(__linux__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "Constants.hpp"
#include "SpriteSet.hpp"

class Player {
   public:
    std::string playerName;
    SpriteSet spriteSet;
    SDL_Surface* spriteSurface;

    int direction;
    SDL_Rect position;

    int timer;
    PlayerStateEnum state;

    Player(const std::string& playerName)
        : playerName(playerName),
          spriteSet("Jogador1")  // Initialize spriteSet here
    {
        state = PlayerStateEnum::IDLE;
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

    void loadSprite(const std::string& spriteFileName) {
        spriteSurface = SDL_LoadBMP(spriteFileName.c_str());
        SDL_SetColorKey(spriteSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                        (Uint16)SDL_MapRGB(spriteSurface->format, 0, 255,
                                           0));  // para por transparencia no verde.
    };

    void setDirection(DirectionEnum directionEnum) { direction = static_cast<int>(directionEnum); }

   private:
};

#endif  // PLAYER_H