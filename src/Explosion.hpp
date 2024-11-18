// Player.hpp
#ifndef EXPLOSION_H
#define EXPLOSION_H

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

class Explosion {
   public:
    SpriteSet spriteSet;
    SDL_Surface* spriteSurface;
    SDL_Texture* spriteTexture;

    int frame;

    Explosion(const std::string& playerName)
        : spriteSet("Explosion")  // Initialize spriteSet here
    {
        frame = 0;
        spriteSet.addRect("FIRST", 0, 0, 32, 32);
        spriteSet.addRect("SECOND", 34, 0, 32, 32);
        spriteSet.addRect("THIRD", 0, 34, 32, 32);
        spriteSet.addRect("FORTH", 34, 34, 32, 32);
    };

    ~Explosion() { SDL_DestroyTexture(spriteTexture); }

    void loadSprite(const std::string& spriteFileName, SDL_Renderer* renderer) {
        spriteSurface = SDL_LoadBMP(spriteFileName.c_str());
        SDL_SetColorKey(spriteSurface, SDL_TRUE, SDL_MapRGB(spriteSurface->format, 0, 255, 0));
        spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);
    };

   private:
};

#endif  // EXPLOSION_H