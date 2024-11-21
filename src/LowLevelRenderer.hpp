// LowLevelRenderer.hpp
#ifndef LOW_LEVEL_RENDERER_H
#define LOW_LEVEL_RENDERER_H

#include <stdlib.h>

#include <string>

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <windows.h>
#include <winsock2.h>
#elif defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include "Constants.hpp"
#include "SpriteSet.hpp"

// *****************************
// Low level rendering functions
// *****************************

SDL_Texture* createTextureFromBMPWithGreenBG(SDL_Renderer* renderer,
                                             const std::string& spriteFileName);

SDL_Texture* createTextureFromBMP(SDL_Renderer* renderer, const std::string& spriteFileName);

void drawPlayer(int playerDirection, SDL_Rect position, SDL_Texture* playerTexture,
                SpriteSet& playerSpriteSet, SDL_Renderer* renderer);

void drawExplosion(int explosionFrame, SDL_Rect position, SDL_Texture* explosionTexture,
                   SpriteSet& explosionSpriteSet, SDL_Renderer* renderer);

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y,
                SDL_Color color);

void renderInteger(SDL_Renderer* renderer, TTF_Font* font, int number, int x, int y,
                   SDL_Color color);

#endif  // LOW_LEVEL_RENDERER_H