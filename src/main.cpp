#include <stdlib.h>

#include <iostream>
#include <vector>
#include <algorithm>

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
                    const SDL_Rect& menu, std::vector<Projectile>& projectiles, const bool shouldErase) {
    bool notCollidedWithProjectile = true;
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
    SDL_Event& event, int& quit1, int& quit2,
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
                        quit2 = 1;
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
            quit1 = 1;
            quit2 = 1;
        }
    }
}

// *******************************
// Scenes
// *******************************


void runTitleScreen(
    SDL_Texture *titleScreenTexture, SDL_Renderer *renderer, SDL_Event& event,
    int& quit1, int& quit2, int& entrar
) {
    SDL_RenderClear(renderer);

    while (SDL_PollEvent(&event))  // condi��o de entrada (enter)
    {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                    quit2 = 0;
                    entrar = 3;
                    break;
                default:
                    break;
            }
        }
        if (event.type == SDL_QUIT)  // para fechar o programa clicando no X
        {
            quit1 = 1;
        }
    }
    SDL_RenderCopy(renderer, titleScreenTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}

void runMainMenu (
    SDL_Texture *menuSurfaceTexture, SDL_Renderer *renderer, SDL_Event& event,
    int& quit1, int& entrar) {

    SDL_RenderClear(renderer);

    while (SDL_PollEvent(&event))  // menu do jogo.
    {
        if (event.type == SDL_KEYDOWN)  // evento do tipo tecla pressionada
        {
            switch (event.key.keysym.sym) {
                case SDLK_1:
                    entrar = 4;
                    break;
                case SDLK_2:
                    entrar = 2;
                    break;
                case SDLK_3:
                    entrar = 1;
                    break;
                default:
                    break;
            }
        }
        if (event.type == SDL_QUIT)  // fechar o programa caso cliquem no X
        {
            quit1 = 1;
        }
    }

    SDL_RenderCopy(renderer, menuSurfaceTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Init(SDL_INIT_TIMER);    // inicializa��o das fun��es de tempo

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // SDL_Surface* titleScreen = nullptr;
    // SDL_Surface* menuSurface = nullptr;
    SDL_Surface* winnerScreenSurface = nullptr;
    // SDL_Surface* backgroundSurface = nullptr;
    // SDL_Surface* projectileSurface = nullptr;
    // SDL_Surface* explosao = nullptr;
    // SDL_Surface* pontuacao = nullptr;

    // SpriteSet pontuacao01{"Pontuacao01"};
    Player player1{"Player1"};
    Player player2{"Player2"};

    std::vector<Projectile> projectiles{};
    size_t projectiles_counter = 0;

    SDL_Rect wall;  // retangulo para determinar a colis�o com a parede central
    SDL_Rect menu;  // retangulo para determinar a colis�o com a barra de informa��es inferior
    SDL_Rect offset3;      // coordenada da bala
    SDL_Rect pontuacao02;  // coordenada da pontua��o do jogador 1
    SDL_Rect pontuacao03;  // coordenada da pontua��o do jogador 2
    SDL_Rect bala01;
    SDL_Rect vencedor;  // coordenada do numero do vencedor
    SDL_Event event;    // variavel para determina��o de eventos

    int quit1, quit2, x, entrar, bala1, bala2, pontuacao1, pontuacao2,
        vencedor0;

    quit1 = 0;
    quit2 = 0;
    entrar = 1;

    SDL_Window *window = SDL_CreateWindow("Campo de Batalha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    player1.loadSprite("resources/jogador1.bmp", renderer);
    player2.loadSprite("resources/jogador2.bmp", renderer);
    SDL_Texture *projectileTexture = createTextureFromBMPWithGreenBG(renderer, "resources/bala.bmp");
    SDL_Texture *explosionTexture = createTextureFromBMPWithGreenBG(renderer, "resources/explosao.bmp");
    SDL_Texture *titleScreenTexture = createTextureFromBMP(renderer, "resources/telainicial.bmp");
    SDL_Texture *menuSurfaceTexture = createTextureFromBMP(renderer, "resources/menu.bmp");
    SDL_Texture *backgroundTexture = createTextureFromBMP(renderer, "resources/fundo.bmp");
    SDL_Texture *winnerScreenTexture = createTextureFromBMP(renderer, "resources/telavencedor.bmp");

    // Load a font
    TTF_Font* font = TTF_OpenFont("resources/armalite_rifle.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    while (quit1 == 0)  // loop principal
    {
        if (entrar == 1)  // condi��o 1 (para tela inicial)
        {
            runTitleScreen(
                titleScreenTexture, renderer, event,
                quit1, quit2, entrar
            );
        } else if (entrar == 3)  // condi��o de entrada no menu
        {
            runMainMenu(menuSurfaceTexture, renderer, event, quit1, entrar);
        } else if (entrar == 2)  // condicao 2 (o jogo em si)
        {
            player1.resetStates();
            player2.resetStates();
            projectiles.clear();
            projectiles.shrink_to_fit();

            // atribuicao de variaveis inteiras , retangulos , coordenadas e etc.
            pontuacao1 = 0;
            pontuacao2 = 0;
            vencedor.x = 233, vencedor.y = 103;

            pontuacao02.x = 212, pontuacao02.y = 435;
            pontuacao03.x = 550, pontuacao03.y = 435;

            player1.position.x = 15, player1.position.y = 170, player1.position.w = 32,
            player1.position.h = 32;
            player2.position.x = 560, player2.position.y = 260, player2.position.w = 32,
            player2.position.h = 32;

            wall.x = 287, wall.y = 102, wall.w = 53, wall.h = 240;
            menu.x = 1, menu.y = 413, menu.w = 638, menu.h = 66;

            bala01.x = 0, bala01.y = 0, bala01.w = 10, bala01.h = 10;

            // Adding rects with names
            // pontuacao01.addRect("ZERO", 0, 3, 25, 35);
            // pontuacao01.addRect("ONE", 50, 3, 25, 35);
            // pontuacao01.addRect("TWO", 98, 3, 25, 35);
            // pontuacao01.addRect("THREE", 147, 3, 25, 35);
            // pontuacao01.addRect("FOUR", 195, 3, 25, 35);
            // pontuacao01.addRect("FIVE", 245, 3, 25, 35);
            // pontuacao01.addRect("SIX", 294, 3, 25, 35);

            // SDL_Rect pontuacaoZeroRect = pontuacao01.getRect("ZERO");
            // SDL_RenderCopy(renderer, pontuacaoTexture, &pontuacaoZeroRect, &pontuacao02);
            // SDL_RenderCopy(renderer, pontuacaoTexture, &pontuacaoZeroRect, &pontuacao03);

            SDL_RenderClear(renderer);

            SDL_Rect playerSpriteRect;

            player1.setDirection(DirectionEnum::RIGHT);
            playerSpriteRect = player1.spriteSet.getRect("RIGHT");
            // SDL_BlitSurface(player1.spriteSurface, &playerSpriteRect, screen, &player1.position);
            SDL_RenderCopy(renderer, player1.spriteTexture, &playerSpriteRect, &player1.position);

            player2.setDirection(DirectionEnum::LEFT);
            playerSpriteRect = player2.spriteSet.getRect("LEFT");
            SDL_RenderCopy(renderer, player2.spriteTexture, &playerSpriteRect, &player2.position);
            // SDL_BlitSurface(player2.spriteSurface, &playerSpriteRect, screen, &player2.position);
            SDL_RenderPresent(renderer);

            SDL_KeyCode lastPressedKeyPlayer1 = SDL_KeyCode::SDLK_UNKNOWN;
            SDL_KeyCode lastPressedKeyPlayer2 = SDL_KeyCode::SDLK_UNKNOWN;

            std::vector<SDL_KeyCode> pressedKeysPlayer1;
            std::vector<SDL_KeyCode> pressedKeysPlayer2;

            while (quit2 == 0)  // loop para controlar o fim do jogo
            {
                Uint32 frameStart = SDL_GetTicks();

                getKeyboardInput(event, quit1, quit2, pressedKeysPlayer1, pressedKeysPlayer2);

                lastPressedKeyPlayer1 = getLastPressedKey(pressedKeysPlayer1);
                const SDL_Rect player1CollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                const SDL_Rect player2CollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                switch (lastPressedKeyPlayer1) {
                    case SDLK_UP: {
                        SDL_Rect movingToCollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                        movingToCollisionBox.y = movingToCollisionBox.y - PLAYER_MOVING_UNITS;
                        if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu, projectiles, false)) {
                            player1.tryWalkUp();
                        }
                        break;
                    }
                    case SDLK_DOWN: {
                        SDL_Rect movingToCollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                        movingToCollisionBox.y = movingToCollisionBox.y + PLAYER_MOVING_UNITS;
                        if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu, projectiles, false)) {
                            player1.tryWalkDown();
                        }
                        break;
                    }
                    case SDLK_RIGHT: {
                        SDL_Rect movingToCollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                        movingToCollisionBox.x = movingToCollisionBox.x + PLAYER_MOVING_UNITS;
                        if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu, projectiles, false)) {
                            player1.tryWalkRight();
                        }
                        break;
                    }
                    case SDLK_LEFT: {
                        SDL_Rect movingToCollisionBox = createCollisionBox(player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                        movingToCollisionBox.x = movingToCollisionBox.x - PLAYER_MOVING_UNITS;
                        if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu, projectiles, false)) {
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
                        if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu, projectiles, false)) {
                            player2.tryWalkUp();
                        }
                        break;
                    }
                    case SDLK_s: {
                        SDL_Rect movingToCollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                        movingToCollisionBox.y = movingToCollisionBox.y + PLAYER_MOVING_UNITS;
                        if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu, projectiles, false)) {
                            player2.tryWalkDown();
                        }
                        break;
                    }
                    case SDLK_d: {
                        SDL_Rect movingToCollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                        movingToCollisionBox.x = movingToCollisionBox.x + PLAYER_MOVING_UNITS;
                        if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu, projectiles, false)) {
                            player2.tryWalkRight();
                        }
                        break;
                    }
                    case SDLK_a: {
                        SDL_Rect movingToCollisionBox = createCollisionBox(player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                        movingToCollisionBox.x = movingToCollisionBox.x - PLAYER_MOVING_UNITS;
                        if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu, projectiles, false)) {
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
                    !checkCollision(player1CollisionBox, player2CollisionBox, wall, menu, projectiles, true)) {
                    pontuacao2 = pontuacao2 + 1;
                    player1.explodingState = PlayerStateEnum::EXPLODING;
                    player1.explodingTimer = PLAYER_EXPLODING_TIMER;
                }

                if (player2.explodingState != PlayerStateEnum::EXPLODING &&
                    !checkCollision(player2CollisionBox, player1CollisionBox, wall, menu, projectiles, true)) {
                    pontuacao1 = pontuacao1 + 1;
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

                renderInteger(renderer, font, pontuacao2, pontuacao03.x, pontuacao03.y);
                renderInteger(renderer, font, pontuacao1, pontuacao02.x, pontuacao02.y);

                SDL_RenderPresent(renderer);

                // Calculate the time taken for the frame
                Uint32 frameTime = SDL_GetTicks() - frameStart;

                // Delay to maintain the desired frame rate, if necessary
                if (frameTime < FRAME_DELAY) {
                    SDL_Delay(FRAME_DELAY - frameTime);
                }

                // condi��es para verificar a vitoria dos jogadores
                if (pontuacao1 == 6)  // verificar vitoria do jogador 1
                {
                    SDL_RenderClear(renderer);

                    vencedor0 = 1;
                    SDL_Delay(1000);
                    quit2 = 1;
                    // telaprincipal = SDL_LoadBMP("resources/telavencedor.bmp");
                    // SDL_BlitSurface(telaprincipal, NULL, screen, NULL);

                    SDL_RenderCopy(renderer, winnerScreenTexture, NULL, NULL);

                    SDL_RenderPresent(renderer);
                    SDL_Delay(2000);
                    while (entrar != 1) {
                        if (SDL_PollEvent(&event))  // verificar caso alguma tecla seja pressionada
                        {
                            if (event.type == SDL_KEYDOWN) {
                                switch (event.key.keysym.sym) {
                                    default:
                                        entrar = 1;
                                }
                            }
                            if (event.type == SDL_QUIT) {
                                quit1 = 1;
                            }
                        }
                    }
                }
                if (pontuacao2 == 6)  // verificar vitoria do jogador 2
                {

                    SDL_RenderClear(renderer);

                    vencedor0 = 2;
                    SDL_Delay(1000);
                    quit2 = 1;

                    SDL_RenderCopy(renderer, winnerScreenTexture, NULL, NULL);

                    SDL_RenderPresent(renderer);
                    SDL_Delay(2000);
                    while (entrar != 1) {
                        if (SDL_PollEvent(&event))  // verificar caso alguma tecla seja pressionada
                        {
                            if (event.type == SDL_KEYDOWN) {
                                switch (event.key.keysym.sym) {
                                    default:
                                        entrar = 1;
                                }
                            }
                            if (event.type == SDL_QUIT) {
                                quit1 = 1;
                            }
                        }
                    }
                }
            }
        } else {
            entrar = 1;
        }
    }
    SDL_Quit();
    return 0;
}
