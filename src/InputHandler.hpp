// InputHandler.hpp
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <algorithm>
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

void getKeyboardInput(SDL_Event& event, bool& quitApp, bool& quitScene,
                      std::vector<SDL_KeyCode>& pressedKeysPlayer1,
                      std::vector<SDL_KeyCode>& pressedKeysPlayer2);

SDL_KeyCode getLastPressedKey(std::vector<SDL_KeyCode>& pressedKeys);

#endif  // INPUT_HANDLER_H