// Player.hpp
#ifndef PROJECTILE_H
#define PROJECTILE_H

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

class Projectile {
   public:
    int projectileId;
    SpriteSet spriteSet;
    SDL_Surface* spriteSurface;
    SDL_Texture* spriteTexture;

    int direction;
    SDL_Rect position;

    int movingCooldown;

    Projectile(const int projectileId)
        : projectileId(projectileId),
          spriteSet("Bala")  // Initialize spriteSet here
    {
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

    ~Projectile() { SDL_DestroyTexture(spriteTexture); }

    void loadSprite(const std::string& spriteFileName, SDL_Renderer* renderer) {
        spriteSurface = SDL_LoadBMP(spriteFileName.c_str());
        SDL_SetColorKey(spriteSurface, SDL_TRUE, SDL_MapRGB(spriteSurface->format, 0, 255, 0));
        spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);
    };

    void setDirection(DirectionEnum directionEnum) { direction = static_cast<int>(directionEnum); }

   private:
};

#endif  // PROJECTILE_H