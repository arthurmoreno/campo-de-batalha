#include <stdlib.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#define SDL_MAIN_HANDLED  // Add this line

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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
#include "Player.hpp"
#include "Projectile.hpp"
#include "SpriteSet.hpp"


// *****************************
// Low level rendering functions
// *****************************

SDL_Texture* createTextureFromBMPWithGreenBG(SDL_Renderer *renderer, const std::string& spriteFileName){
    SDL_Surface* tmpSurface = nullptr;
    tmpSurface = SDL_LoadBMP(spriteFileName.c_str());
    SDL_SetColorKey(
        tmpSurface, SDL_TRUE,
        SDL_MapRGB(tmpSurface->format, 0, 255, 0));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return texture;
}

SDL_Texture* createTextureFromBMP(SDL_Renderer *renderer, const std::string& spriteFileName){
    SDL_Surface* tmpSurface = nullptr;
    tmpSurface = SDL_LoadBMP(spriteFileName.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return texture;
}

// funcao para carregar os tanques na tela
void drawPlayer(int playerPosition, SDL_Rect position, SDL_Texture *playerTexture,
                  SpriteSet& playerSpriteSet, SDL_Renderer *renderer) {
    SDL_Rect playerSpriteRect;
    switch (playerPosition) {
        case static_cast<int>(DirectionEnum::UP):
            playerSpriteRect = playerSpriteSet.getRect("UP");
            break;
        case static_cast<int>(DirectionEnum::DOWN):
            playerSpriteRect = playerSpriteSet.getRect("DOWN");
            break;
        case static_cast<int>(DirectionEnum::RIGHT):
            playerSpriteRect = playerSpriteSet.getRect("RIGHT");
            break;
        case static_cast<int>(DirectionEnum::LEFT):
            playerSpriteRect = playerSpriteSet.getRect("LEFT");
            break;
        default:
            playerSpriteRect = playerSpriteSet.getRect("DOWN");
            break;
    }
    SDL_RenderCopy(renderer, playerTexture, &playerSpriteRect, &position);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {

    // Render text to an SDL_Surface
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!textSurface) {
        SDL_Log("TTF_RenderUTF8_Blended Error: %s", TTF_GetError());
        return;
    }

    // Convert SDL_Surface to SDL_Texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Set destination rectangle for rendering the text
    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};

    // Render the texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Function to render an integer as text
void renderInteger(SDL_Renderer* renderer, TTF_Font* font, int number, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};  // White color for text

    // Convert integer to string
    std::string text = std::to_string(number);


    // Render text to an SDL_Surface
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!textSurface) {
        SDL_Log("TTF_RenderUTF8_Blended Error: %s", TTF_GetError());
        return;
    }

    // Convert SDL_Surface to SDL_Texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Set destination rectangle for rendering the text
    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};

    // Render the texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


// *****************************
// Audio utils functions
// *****************************

struct Track {
    Uint8* start;
    Uint32 length;
};

struct AudioData {
    Track* tracks;     // Array of tracks
    int numTracks;     // Number of tracks
    int currentTrack;  // Index of the current track
    Uint8* pos;        // Current position in the current track
    Uint32 remaining;  // Remaining length in the current track
};

void audioCallback(void* userdata, Uint8* stream, int len) {
    AudioData* audio = static_cast<AudioData*>(userdata);

    Uint32 length = static_cast<Uint32>(len);

    if (audio->remaining == 0) {
        // Choose a random track
        audio->currentTrack = rand() % audio->numTracks;
        audio->pos = audio->tracks[audio->currentTrack].start;
        audio->remaining = audio->tracks[audio->currentTrack].length;
    }

    Uint32 lenToCopy = (length > audio->remaining) ? audio->remaining : length;
    SDL_memcpy(stream, audio->pos, lenToCopy);

    audio->pos += lenToCopy;
    audio->remaining -= lenToCopy;

    // If we didn't fill the entire buffer, pad the rest with silence
    if (lenToCopy < length) {
        SDL_memset(stream + lenToCopy, 0, length - lenToCopy);
    }
}

// *******************************
// Collision and Physics functions
// *******************************

// Function to create a collision box based on an original SDL_Rect
SDL_Rect createCollisionBox(const SDL_Rect& originalRect, int shrinkX, int shrinkY) {
    SDL_Rect collisionBox;

    // Adjust the position to center the collision box
    collisionBox.x = originalRect.x + shrinkX;
    collisionBox.y = originalRect.y + shrinkY;

    // Adjust the width and height
    collisionBox.w = originalRect.w - shrinkX * 2;
    collisionBox.h = originalRect.h - shrinkY * 2;

    // Ensure the width and height are not negative
    if (collisionBox.w < 0) collisionBox.w = 0;
    if (collisionBox.h < 0) collisionBox.h = 0;

    return collisionBox;
}

// funcao que verifica a colisao entre objetos
bool checkCollisionBetweenTwoRects(int ax, int ay, int aw, int ah, int cx, int cy, int cw, int ch) {
    int bx, by, dx, dy;
    bx = ax + aw;
    by = ay + ah;
    dx = cx + cw;
    dy = cy + ch;
    return (((ax > dx) || (bx < cx) || (ay > dy) || (by < cy)));
}

bool checkCollision(const SDL_Rect& firstPlayerPosition, const SDL_Rect& secondPlayerPosition, const SDL_Rect& wall,
                    const SDL_Rect& menu, std::vector<Projectile>& projectiles,
                    const bool checkProjectiles, const bool shouldErase) {
    bool notCollidedWithProjectile = true;
    if (checkProjectiles) {
        for (auto projectile = projectiles.begin(); projectile != projectiles.end();) {
            bool _notCollidedWithProjectile = checkCollisionBetweenTwoRects(
                firstPlayerPosition.x, firstPlayerPosition.y, firstPlayerPosition.w,
                firstPlayerPosition.h, projectile->position.x, projectile->position.y,
                projectile->position.w, projectile->position.h);
            std::cout << "Not collided with projectile: " << _notCollidedWithProjectile << std::endl;
            notCollidedWithProjectile = (notCollidedWithProjectile && _notCollidedWithProjectile);

            if (!_notCollidedWithProjectile && shouldErase) {
                // Collision detected, remove projectile
                projectile = projectiles.erase(projectile);  // Erase returns the next iterator
            } else {
                ++projectile;  // Move to the next element
            }
        }
    }

    return (checkCollisionBetweenTwoRects(firstPlayerPosition.x, firstPlayerPosition.y,
                                          firstPlayerPosition.w, firstPlayerPosition.h, wall.x,
                                          wall.y, wall.w, wall.h) &&
            checkCollisionBetweenTwoRects(firstPlayerPosition.x, firstPlayerPosition.y,
                                          firstPlayerPosition.w, firstPlayerPosition.h,
                                          secondPlayerPosition.x, secondPlayerPosition.y,
                                          secondPlayerPosition.w, secondPlayerPosition.h) &&
            checkCollisionBetweenTwoRects(firstPlayerPosition.x, firstPlayerPosition.y,
                                          firstPlayerPosition.w, firstPlayerPosition.h, menu.x,
                                          menu.y, menu.w, menu.h) &&
            notCollidedWithProjectile);
}

void fireProjectile(Player& player, std::vector<Projectile>& projectiles, size_t& projectiles_counter) {
    if (player.firingState == PlayerStateEnum::IDLE) {
        Projectile projectile = Projectile(projectiles_counter);

        switch (player.direction) {
            case static_cast<int>(DirectionEnum::UP): {
                projectile.position.x = player.position.x + 12;
                projectile.position.y = player.position.y - 20;
                projectile.position.w = 12;
                projectile.position.h = 12;
                projectile.setDirection(DirectionEnum::UP);

                player.firingState = PlayerStateEnum::FIRING;
                player.firingCooldown = PLAYER_FIRING_TIMER;
            } break;
            case static_cast<int>(DirectionEnum::DOWN): {
                projectile.position.x = player.position.x + 12;
                projectile.position.y = player.position.y + 40;
                projectile.position.w = 12;
                projectile.position.h = 12;
                projectile.setDirection(DirectionEnum::DOWN);

                player.firingState = PlayerStateEnum::FIRING;
                player.firingCooldown = PLAYER_FIRING_TIMER;
            } break;
            case static_cast<int>(DirectionEnum::RIGHT): {
                projectile.position.x = player.position.x + 40;
                projectile.position.y = player.position.y + 12;
                projectile.position.w = 12;
                projectile.position.h = 12;
                projectile.setDirection(DirectionEnum::RIGHT);

                player.firingState = PlayerStateEnum::FIRING;
                player.firingCooldown = PLAYER_FIRING_TIMER;
            } break;
            case static_cast<int>(DirectionEnum::LEFT): {
                projectile.position.x = player.position.x - 20;
                projectile.position.y = player.position.y + 12;
                projectile.position.w = 12;
                projectile.position.h = 12;
                projectile.setDirection(DirectionEnum::LEFT);

                player.firingState = PlayerStateEnum::FIRING;
                player.firingCooldown = PLAYER_FIRING_TIMER;
            } break;

            default:
                break;
        }

        projectile.movingCooldown = PROJECTILE_MOVING_COOLDOWN;
        projectiles.push_back(projectile);
        projectiles_counter++;
    }
}

void processProjectilesPhysics(std::vector<Projectile>& projectiles) {
    for (auto projectile = projectiles.begin(); projectile != projectiles.end();) {
        if (projectile->movingCooldown > 0) {
            projectile->movingCooldown--;
        } else {
            switch (projectile->direction) {
                case static_cast<int>(DirectionEnum::UP): {
                    projectile->position.y = projectile->position.y - PROJECTILE_MOVING_UNITS;
                    projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                } break;
                case static_cast<int>(DirectionEnum::DOWN): {
                    projectile->position.y = projectile->position.y + PROJECTILE_MOVING_UNITS;
                    projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                } break;
                case static_cast<int>(DirectionEnum::RIGHT): {
                    projectile->position.x = projectile->position.x + PROJECTILE_MOVING_UNITS;
                    projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                } break;
                case static_cast<int>(DirectionEnum::LEFT): {
                    projectile->position.x = projectile->position.x - PROJECTILE_MOVING_UNITS;
                    projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                } break;

                default:
                    break;
            }
        }
        if (projectile->position.x < -10 || projectile->position.x > 1000 ||
            projectile->position.y < -10 || projectile->position.y > 1000) {
            projectile =
                projectiles.erase(projectile);  // erase returns the next valid iterator
        } else {
            ++projectile;  // Only increment if no deletion occurred
        }
    }
}

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

void getKeyboardInput(
    SDL_Event& event, bool& quitApp, bool& quitScene,
    std::vector<SDL_KeyCode>& pressedKeysPlayer1,
    std::vector<SDL_KeyCode>& pressedKeysPlayer2
){
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

// *******************************
// Scenes
// *******************************


void runTitleScreen(
    SDL_Texture *titleScreenTexture, SDL_Renderer *renderer, SDL_Event& event,
    bool& quitApp, SceneSelection& selectedScene,
    TTF_Font* fontArmaliteRifle24, TTF_Font* fontArmaliteRifle50, 
    SDL_Color whiteColor, SDL_Color blackColor
) {

    Uint32 pressEnterCounter = 0;

    bool quitScene = false;
    while (!quitScene)  // loop para controlar o fim do jogo
    {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event))  // condi��o de entrada (enter)
        {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        quitScene = true;
                        selectedScene = SceneSelection::MAIN_MENU;
                        break;
                    default:
                        break;
                }
            }
            if (event.type == SDL_QUIT)  // para fechar o programa clicando no X
            {
                quitScene = true;
                quitApp = true;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, titleScreenTexture, NULL, NULL);

        renderText(renderer, fontArmaliteRifle50, "Campo de Batalha", 80, 60, blackColor);
        if (pressEnterCounter >= 0 && pressEnterCounter < 50) {
            renderText(renderer, fontArmaliteRifle24, "Press Enter!", 240, 280, blackColor);
        }
        pressEnterCounter = (pressEnterCounter >= 75) ? 0 : pressEnterCounter + 1;

        SDL_RenderPresent(renderer);

        // Calculate the time taken for the frame
        Uint32 frameTime = SDL_GetTicks() - frameStart;

        // Delay to maintain the desired frame rate, if necessary
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
}

void runMainMenu (
    SDL_Renderer *renderer, SDL_Event& event,
    bool& quitApp, SceneSelection& selectedScene,
    SDL_Texture *titleScreenTexture, SDL_Texture *projectileTexture,
    TTF_Font* fontArmaliteRifle24, TTF_Font* fontArmaliteRifle50, 
    SDL_Color whiteColor, SDL_Color blackColor
) {
    const int GAME_MATCH_OPTION_X = 200;
    const int GAME_MATCH_OPTION_Y = 270;

    const int TITLE_SCREEN_OPTION_X = 200;
    const int TITLE_SCREEN_OPTION_Y = 300;

    const int BULLET_OFFSET_X = 25;
    const int BULLET_OFFSET_Y = 7;

    Uint32 pressEnterCounter = 0;
    bool quitScene = false;
    enum struct Options { GAME_MATCH = 0, TITLE_SCREEN = 1, UNKNOWN = 2};

    Options selectedOption = Options::GAME_MATCH;
    SDL_Rect blitPosition;
    blitPosition.x = GAME_MATCH_OPTION_X - BULLET_OFFSET_X;
    blitPosition.y = GAME_MATCH_OPTION_Y + BULLET_OFFSET_Y;
    blitPosition.w = 12;
    blitPosition.h = 12;
    while (!quitScene)  // loop para controlar o fim do jogo
    {
        Uint32 frameStart = SDL_GetTicks();


        while (SDL_PollEvent(&event))  // menu do jogo.
        {
            if (event.type == SDL_KEYDOWN)  // evento do tipo tecla pressionada
            {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (selectedOption == Options::GAME_MATCH) {
                            selectedOption = Options::TITLE_SCREEN;
                            blitPosition.y = TITLE_SCREEN_OPTION_Y + BULLET_OFFSET_Y;
                        } else {
                            selectedOption = Options::GAME_MATCH;
                            blitPosition.y = GAME_MATCH_OPTION_Y + BULLET_OFFSET_Y;
                        }
                        break;
                    case SDLK_DOWN:
                        if (selectedOption == Options::GAME_MATCH) {
                            selectedOption = Options::TITLE_SCREEN;
                            blitPosition.y = TITLE_SCREEN_OPTION_Y + BULLET_OFFSET_Y;
                        } else {
                            selectedOption = Options::GAME_MATCH;
                            blitPosition.y = GAME_MATCH_OPTION_Y + BULLET_OFFSET_Y;
                        }
                        break;
                    case SDLK_RETURN:
                        quitScene = true;
                        if (selectedOption == Options::GAME_MATCH) {
                            selectedScene = SceneSelection::GAME_MATCH;
                        } else {
                            selectedScene = SceneSelection::TITLE_SCREEN;
                        }
                        break;
                    default:
                        break;
                }
            }
            if (event.type == SDL_QUIT)  // fechar o programa caso cliquem no X
            {
                quitScene = true;
                quitApp = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, titleScreenTexture, NULL, NULL);

        renderText(renderer, fontArmaliteRifle50, "Campo de Batalha", 80, 60, blackColor);
        renderText(renderer, fontArmaliteRifle24, "Start Match", GAME_MATCH_OPTION_X, GAME_MATCH_OPTION_Y, blackColor);
        renderText(renderer, fontArmaliteRifle24, "Back to title Screen", TITLE_SCREEN_OPTION_X, TITLE_SCREEN_OPTION_Y, blackColor);
        if (pressEnterCounter >= 0 && pressEnterCounter < 50) {

            SDL_RenderCopy(renderer, projectileTexture, NULL, &blitPosition);
        }
        pressEnterCounter = (pressEnterCounter >= 75) ? 0 : pressEnterCounter + 1;

        SDL_RenderPresent(renderer);

        // Calculate the time taken for the frame
        Uint32 frameTime = SDL_GetTicks() - frameStart;

        // Delay to maintain the desired frame rate, if necessary
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
}

void runGameMatch(
    SDL_Renderer *renderer, SDL_Event& event, TTF_Font* fontArmaliteRifle24,
    Player& player1, Player& player2, 
    SDL_Texture *projectileTexture, SDL_Texture *explosionTexture, SDL_Texture *backgroundTexture,
    SceneSelection& selectedScene, int& winnerIdNumber, bool& quitApp
) {

    std::vector<Projectile> projectiles{};
    size_t projectiles_counter = 0;

    int scorePlayer1 = 0, scorePlayer2 = 0;

    player1.resetStates();
    player2.resetStates();

    SDL_Rect wall;
    SDL_Rect menu;
    SDL_Rect scorePlayer1Position;
    SDL_Rect scorePlayer2Position;

    scorePlayer1Position.x = 212, scorePlayer1Position.y = 435;
    scorePlayer2Position.x = 550, scorePlayer2Position.y = 435;

    player1.position.x = 15, player1.position.y = 170, player1.position.w = 32,
    player1.position.h = 32;
    player2.position.x = 560, player2.position.y = 260, player2.position.w = 32,
    player2.position.h = 32;

    wall.x = 287, wall.y = 102, wall.w = 53, wall.h = 240;
    menu.x = 1, menu.y = 413, menu.w = 638, menu.h = 66;

    SDL_RenderClear(renderer);

    SDL_Rect playerSpriteRect;

    player1.setDirection(DirectionEnum::RIGHT);
    playerSpriteRect = player1.spriteSet.getRect("RIGHT");
    SDL_RenderCopy(renderer, player1.spriteTexture, &playerSpriteRect, &player1.position);

    player2.setDirection(DirectionEnum::LEFT);
    playerSpriteRect = player2.spriteSet.getRect("LEFT");
    SDL_RenderCopy(renderer, player2.spriteTexture, &playerSpriteRect, &player2.position);
    SDL_RenderPresent(renderer);

    SDL_KeyCode lastPressedKeyPlayer1 = SDL_KeyCode::SDLK_UNKNOWN;
    SDL_KeyCode lastPressedKeyPlayer2 = SDL_KeyCode::SDLK_UNKNOWN;

    std::vector<SDL_KeyCode> pressedKeysPlayer1;
    std::vector<SDL_KeyCode> pressedKeysPlayer2;

    bool quitScene = false;

    while (!quitScene)  // loop para controlar o fim do jogo
    {
        Uint32 frameStart = SDL_GetTicks();

        getKeyboardInput(event, quitApp, quitScene, pressedKeysPlayer1, pressedKeysPlayer2);

        lastPressedKeyPlayer1 = getLastPressedKey(pressedKeysPlayer1);
        const SDL_Rect player1CollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
        const SDL_Rect player2CollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
        switch (lastPressedKeyPlayer1) {
            case SDLK_UP: {
                SDL_Rect movingToCollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.y = movingToCollisionBox.y - PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu, projectiles, false, false)) {
                    player1.tryWalkUp();
                }
                break;
            }
            case SDLK_DOWN: {
                SDL_Rect movingToCollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.y = movingToCollisionBox.y + PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu, projectiles, false, false)) {
                    player1.tryWalkDown();
                }
                break;
            }
            case SDLK_RIGHT: {
                SDL_Rect movingToCollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.x = movingToCollisionBox.x + PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu, projectiles, false, false)) {
                    player1.tryWalkRight();
                }
                break;
            }
            case SDLK_LEFT: {
                SDL_Rect movingToCollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.x = movingToCollisionBox.x - PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu, projectiles, false, false)) {
                    player1.tryWalkLeft();
                }
                break;
            }
            case SDLK_m:
            {
                fireProjectile(player1, projectiles, projectiles_counter);
            } break;
            default:
                break;
        }

        lastPressedKeyPlayer2 = getLastPressedKey(pressedKeysPlayer2);
        switch (lastPressedKeyPlayer2) {
            case SDLK_w: {
                SDL_Rect movingToCollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.y = movingToCollisionBox.y - PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu, projectiles, false, false)) {
                    player2.tryWalkUp();
                }
                break;
            }
            case SDLK_s: {
                SDL_Rect movingToCollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.y = movingToCollisionBox.y + PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu, projectiles, false, false)) {
                    player2.tryWalkDown();
                }
                break;
            }
            case SDLK_d: {
                SDL_Rect movingToCollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.x = movingToCollisionBox.x + PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu, projectiles, false, false)) {
                    player2.tryWalkRight();
                }
                break;
            }
            case SDLK_a: {
                SDL_Rect movingToCollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.x = movingToCollisionBox.x - PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu, projectiles, false, false)) {
                    player2.tryWalkLeft();
                }
                break;
            }
            case SDLK_f:
            {
                fireProjectile(player2, projectiles, projectiles_counter);
            } break;
            default:
                break;
        }

        if (player1.explodingState != PlayerStateEnum::EXPLODING &&
            !checkCollision(player1CollisionBox, player2CollisionBox, wall, menu, projectiles, true, true)) {
            scorePlayer2++;
            player1.explodingState = PlayerStateEnum::EXPLODING;
            player1.explodingTimer = PLAYER_EXPLODING_TIMER;
        }

        if (player2.explodingState != PlayerStateEnum::EXPLODING &&
            !checkCollision(player2CollisionBox, player1CollisionBox, wall, menu, projectiles, true, true)) {
            scorePlayer1++;
            player2.explodingState = PlayerStateEnum::EXPLODING;
            player2.explodingTimer = PLAYER_EXPLODING_TIMER;
        }

        player1.processAllStates();
        player2.processAllStates();
        processProjectilesPhysics(projectiles);

        SDL_RenderClear(renderer);

        // SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        drawPlayer(player1.direction, player1.position, player1.spriteTexture,
                        player1.spriteSet, renderer);

        if (player1.explodingState == PlayerStateEnum::EXPLODING) {
            SDL_RenderCopy(renderer, explosionTexture, NULL, &player1.position);
        }

        drawPlayer(player2.direction, player2.position, player2.spriteTexture,
                        player2.spriteSet, renderer);

        if (player2.explodingState == PlayerStateEnum::EXPLODING) {
            SDL_RenderCopy(renderer, explosionTexture, NULL, &player2.position);
        }

        for (auto projectile = projectiles.begin(); projectile != projectiles.end(); ++projectile) {
            SDL_Rect blitPosition = projectile->position;
            SDL_RenderCopy(renderer, projectileTexture, NULL, &blitPosition);
        }

        renderInteger(renderer, fontArmaliteRifle24, scorePlayer2, scorePlayer2Position.x, scorePlayer2Position.y);
        renderInteger(renderer, fontArmaliteRifle24, scorePlayer1, scorePlayer1Position.x, scorePlayer1Position.y);

        SDL_RenderPresent(renderer);

        // Calculate the time taken for the frame
        Uint32 frameTime = SDL_GetTicks() - frameStart;

        // Delay to maintain the desired frame rate, if necessary
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }

        // condi��es para verificar a vitoria dos jogadores
        if (scorePlayer1 >= 6)  // verificar vitoria do jogador 1
        {
            winnerIdNumber = 2;
            selectedScene = SceneSelection::WINNER_SCREEN;
            quitScene = true;
        }
        if (scorePlayer2 >= 6)  // verificar vitoria do jogador 2
        {
            winnerIdNumber = 2;
            selectedScene = SceneSelection::WINNER_SCREEN;
            quitScene = true;
        }
    }
}

void runWinnerScene(
    SDL_Renderer *renderer, SDL_Event& event, SDL_Texture *winnerScreenTexture, SceneSelection& selectedScene,
    int& winnerIdNumber, bool& quitApp, TTF_Font* fontArmaliteRifle24, SDL_Color blackColor
) {

    Uint32 pressEnterCounter = 0;

    bool quitScene = false;
    bool firstFrame = true;
    while (!quitScene)  // loop para controlar o fim do jogo
    {
        while (SDL_PollEvent(&event))  // verificar caso alguma tecla seja pressionada
        {
            if (event.type == SDL_KEYDOWN && !firstFrame) {
                quitScene = true;
                selectedScene = SceneSelection::MAIN_MENU;
            }
            if (event.type == SDL_QUIT) {
                quitScene = true;
                quitApp = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, winnerScreenTexture, NULL, NULL);

        std::stringstream ss;
        ss << "The winner is Player " << winnerIdNumber;
        renderText(renderer, fontArmaliteRifle24, ss.str().c_str(), 80, 60, blackColor);

        if (pressEnterCounter >= 0 && pressEnterCounter < 50) {
            renderText(renderer, fontArmaliteRifle24, "Press any key!", 240, 280, blackColor);
        }
        pressEnterCounter = (pressEnterCounter >= 75) ? 0 : pressEnterCounter + 1;

        SDL_RenderPresent(renderer);

        if (firstFrame) {
            SDL_Delay(3000);
            firstFrame = false;
        } else {
            SDL_Delay(16);
        }
    }
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Init(SDL_INIT_TIMER);    // inicializa��o das fun��es de tempo

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Player player1{"Player1"};
    Player player2{"Player2"};

    SDL_Event event;    // variavel para determina��o de eventos

    int winnerIdNumber;
    bool quitApp = false, quitScene = false;

    SceneSelection selectedScene = SceneSelection::TITLE_SCREEN;

    SDL_Window *window = SDL_CreateWindow("Campo de Batalha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    player1.loadSprite("resources/jogador1.bmp", renderer);
    player2.loadSprite("resources/jogador2.bmp", renderer);
    SDL_Texture *projectileTexture = createTextureFromBMPWithGreenBG(renderer, "resources/bala.bmp");
    SDL_Texture *explosionTexture = createTextureFromBMPWithGreenBG(renderer, "resources/explosao.bmp");
    SDL_Texture *titleScreenTexture = createTextureFromBMP(renderer, "resources/telainicial.bmp");
    SDL_Texture *menuSurfaceTexture = createTextureFromBMP(renderer, "resources/telainicial.bmp");
    SDL_Texture *backgroundTexture = createTextureFromBMP(renderer, "resources/fundo.bmp");
    SDL_Texture *winnerScreenTexture = createTextureFromBMP(renderer, "resources/telavencedor.bmp");

    // Load a font
    const std::string fontFileName = "resources/armalite_rifle.ttf";
    TTF_Font* fontArmaliteRifle24 = TTF_OpenFont(fontFileName.c_str(), 24);
    if (!fontArmaliteRifle24) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* fontArmaliteRifle50 = TTF_OpenFont(fontFileName.c_str(), 50);
    if (!fontArmaliteRifle24) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color blackColor = {16, 18, 28, 255};


    SDL_AudioSpec wavSpec;
    Track tracks[3];

    // Load the WAV files
    const char* filenames[3] = {
        "resources/songtrack001.wav",
        "resources/songtrack002.wav",
        "resources/songtrack003.wav"
    };

    for (int i = 0; i < 3; ++i) {
        if (SDL_LoadWAV(filenames[i], &wavSpec, &tracks[i].start, &tracks[i].length) == NULL) {
            std::cerr << "Failed to load WAV: " << filenames[i] << " Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return -1;
        }
    }

    AudioData audioData;
    audioData.tracks = tracks;
    audioData.numTracks = 3;
    audioData.currentTrack = 0; // Start with track 0
    audioData.pos = tracks[0].start;
    audioData.remaining = tracks[0].length;

    // Set the audio callback
    wavSpec.callback = audioCallback;
    wavSpec.userdata = &audioData;

    // Open audio device
    if (SDL_OpenAudio(&wavSpec, NULL) < 0) {
        std::cerr << "SDL_OpenAudio error: " << SDL_GetError() << std::endl;
        // Free loaded WAV files
        for (int i = 0; i < 3; ++i) {
            SDL_FreeWAV(tracks[i].start);
        }
        SDL_Quit();
        return -1;
    }

    // Initialize random seed
    srand(static_cast<unsigned int>(time(0)));

    // Start playing audio
    SDL_PauseAudio(0);

    while (!quitApp)  // loop principal
    {
        if (selectedScene == SceneSelection::TITLE_SCREEN)
        {
            runTitleScreen(
                titleScreenTexture, renderer, event,
                quitApp, selectedScene,
                fontArmaliteRifle24, fontArmaliteRifle50, 
                whiteColor, blackColor
            );
        } else if (selectedScene == SceneSelection::MAIN_MENU)
        {
            runMainMenu(
                renderer, event, quitApp, selectedScene,
                titleScreenTexture, projectileTexture,
                fontArmaliteRifle24, fontArmaliteRifle50, whiteColor, blackColor);
        } else if (selectedScene == SceneSelection::GAME_MATCH)  // condicao 2 (o jogo em si)
        {
            runGameMatch(
                renderer, event, fontArmaliteRifle24, player1, player2, 
                projectileTexture, explosionTexture, backgroundTexture,
                selectedScene, winnerIdNumber, quitApp
            );
        } else if (selectedScene == SceneSelection::WINNER_SCREEN) {
            runWinnerScene(
                renderer, event, winnerScreenTexture, selectedScene,
                winnerIdNumber, quitApp, fontArmaliteRifle24, blackColor
            );

        } else {
            selectedScene = SceneSelection::TITLE_SCREEN;
        }
    }

    // Clean up
    SDL_CloseAudio();
    // Free loaded WAV files
    for (int i = 0; i < 3; ++i) {
        SDL_FreeWAV(tracks[i].start);
    }
    SDL_Quit();
    return 0;
}
