// LowLevelRenderer.hpp
#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <SDL2/SDL.h>
#include <windows.h>
#elif defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "Player.hpp"
#include "Projectile.hpp"

SDL_Rect createCollisionBox(const SDL_Rect& originalRect, int shrinkX, int shrinkY);

bool checkCollision(const SDL_Rect& firstPlayerPosition, const SDL_Rect& secondPlayerPosition,
                    const SDL_Rect& wall, const SDL_Rect& menu,
                    std::vector<Projectile>& projectiles, const bool checkProjectiles,
                    const bool shouldErase);

void fireProjectile(Player& player, std::vector<Projectile>& projectiles,
                    size_t& projectiles_counter);

void processProjectilesPhysics(std::vector<Projectile>& projectiles);

#endif  // PHYSICS_H