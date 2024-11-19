#include "InputHandler.hpp"

// *******************************
// Players Input
// *******************************

void addKeyIfNotPressed(std::vector<SDL_KeyCode>& pressedKeys, SDL_KeyCode key) {
    if (std::find(pressedKeys.begin(), pressedKeys.end(), key) == pressedKeys.end()) {
        pressedKeys.push_back(key);
    }
}

void removeKeyFromList(std::vector<SDL_KeyCode>& pressedKeys, SDL_KeyCode key) {
    auto it = std::find(pressedKeys.begin(), pressedKeys.end(), key);
    if (it != pressedKeys.end()) {
        pressedKeys.erase(it);
    }
}

SDL_KeyCode getLastPressedKey(std::vector<SDL_KeyCode>& pressedKeys) {
    if (!pressedKeys.empty()) {
        // Using the index operator
        return pressedKeys.back();
    } else {
        return SDL_KeyCode::SDLK_UNKNOWN;
    }
}

void getKeyboardInput(SDL_Event& event, bool& quitApp, bool& quitScene,
                      std::vector<SDL_KeyCode>& pressedKeysPlayer1,
                      std::vector<SDL_KeyCode>& pressedKeysPlayer2) {
    while (SDL_PollEvent(&event))  // loop de eventos
    {
        switch (event.type) {
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    // Player 1 movement keys
                    case SDLK_UP: {
                        addKeyIfNotPressed(pressedKeysPlayer1, SDL_KeyCode::SDLK_UP);
                        break;
                    }
                    case SDLK_DOWN: {
                        addKeyIfNotPressed(pressedKeysPlayer1, SDL_KeyCode::SDLK_DOWN);
                        break;
                    }
                    case SDLK_RIGHT: {
                        addKeyIfNotPressed(pressedKeysPlayer1, SDL_KeyCode::SDLK_RIGHT);
                        break;
                    }
                    case SDLK_LEFT: {
                        addKeyIfNotPressed(pressedKeysPlayer1, SDL_KeyCode::SDLK_LEFT);
                        break;
                    }
                    // Player 2 movement keys
                    case SDLK_w: {
                        addKeyIfNotPressed(pressedKeysPlayer2, SDL_KeyCode::SDLK_w);
                        break;
                    }
                    case SDLK_s: {
                        addKeyIfNotPressed(pressedKeysPlayer2, SDL_KeyCode::SDLK_s);
                        break;
                    }
                    case SDLK_d: {
                        addKeyIfNotPressed(pressedKeysPlayer2, SDL_KeyCode::SDLK_d);
                        break;
                    }
                    case SDLK_a: {
                        addKeyIfNotPressed(pressedKeysPlayer2, SDL_KeyCode::SDLK_a);
                        break;
                    }
                    case SDLK_ESCAPE:
                        quitScene = true;
                        break;
                    // comandos para o tiro dos jogadores 1 e 2
                    case SDLK_m:  // tecla m para o tiro do jogador 1
                    {
                        addKeyIfNotPressed(pressedKeysPlayer1, SDL_KeyCode::SDLK_m);
                    } break;
                    case SDLK_f:  // tecla f para o jogador 2
                    {
                        addKeyIfNotPressed(pressedKeysPlayer2, SDL_KeyCode::SDLK_f);
                    } break;
                    default:
                        break;
                }
                break;
            }
            case SDL_KEYUP: {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        removeKeyFromList(pressedKeysPlayer1, SDL_KeyCode::SDLK_UP);
                        break;
                    case SDLK_DOWN:  // tecla para baixo
                        removeKeyFromList(pressedKeysPlayer1, SDL_KeyCode::SDLK_DOWN);
                        break;
                    case SDLK_RIGHT:  // tecla para a direita
                        removeKeyFromList(pressedKeysPlayer1, SDL_KeyCode::SDLK_RIGHT);
                        break;
                    case SDLK_LEFT:  // tecla para esquerda
                        removeKeyFromList(pressedKeysPlayer1, SDL_KeyCode::SDLK_LEFT);
                        break;

                    case SDLK_m:
                        removeKeyFromList(pressedKeysPlayer1, SDL_KeyCode::SDLK_m);
                        break;
                    // comandos para o jogador 2
                    case SDLK_w:  // tecla w ( para cima )
                        removeKeyFromList(pressedKeysPlayer2, SDL_KeyCode::SDLK_w);
                        break;
                    case SDLK_s:  // tecla s ( para baixo )
                        removeKeyFromList(pressedKeysPlayer2, SDL_KeyCode::SDLK_s);
                        break;
                    case SDLK_d:  // tecla d ( para direita )
                        removeKeyFromList(pressedKeysPlayer2, SDL_KeyCode::SDLK_d);
                        break;
                    case SDLK_a:  // tecla a ( para esquerda )
                        removeKeyFromList(pressedKeysPlayer2, SDL_KeyCode::SDLK_a);
                        break;

                    case SDLK_f:
                        removeKeyFromList(pressedKeysPlayer2, SDL_KeyCode::SDLK_f);
                        break;
                    default:
                        break;
                }
                break;
            }
        }
        if (event.type == SDL_QUIT)  // fechar o programa caso cliquem no X
        {
            quitApp = true;
            quitScene = true;
        }
    }
}
