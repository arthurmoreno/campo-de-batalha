#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#define SDL_MAIN_HANDLED

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
#include "Explosion.hpp"
#include "InputHandler.hpp"
#include "LowLevelRenderer.hpp"
#include "Physics.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "SpriteSet.hpp"

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
// Scenes
// *******************************

void runTitleScreen(SDL_Texture* titleScreenTexture, SDL_Renderer* renderer, SDL_Event& event,
                    bool& quitApp, SceneSelection& selectedScene, TTF_Font* fontArmaliteRifle24,
                    TTF_Font* fontArmaliteRifle50, SDL_Color whiteColor, SDL_Color blackColor) {
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

void runMainMenu(SDL_Renderer* renderer, SDL_Event& event, bool& quitApp,
                 SceneSelection& selectedScene, SDL_Texture* titleScreenTexture,
                 SDL_Texture* projectileTexture, TTF_Font* fontArmaliteRifle24,
                 TTF_Font* fontArmaliteRifle50, SDL_Color whiteColor, SDL_Color blackColor) {
    const int GAME_MATCH_OPTION_X = 200;
    const int GAME_MATCH_OPTION_Y = 245;

    const int SHOW_CONTROLS_OPTION_X = 200;
    const int SHOW_CONTROLS_OPTION_Y = 275;

    const int TITLE_SCREEN_OPTION_X = 200;
    const int TITLE_SCREEN_OPTION_Y = 305;

    const int BULLET_OFFSET_X = 25;
    const int BULLET_OFFSET_Y = 7;

    const int BULLET_OPTION_X = GAME_MATCH_OPTION_X - BULLET_OFFSET_X;

    const int BULLET_GAME_MATCH_OPTION_Y = GAME_MATCH_OPTION_Y + BULLET_OFFSET_Y;
    const int BULLET_SHOW_CONTROLS_OPTION_Y = SHOW_CONTROLS_OPTION_Y + BULLET_OFFSET_Y;
    const int BULLET_TITLE_SCREEN_OPTION_Y = TITLE_SCREEN_OPTION_Y + BULLET_OFFSET_Y;

    Uint32 pressEnterCounter = 0;
    bool quitScene = false;
    enum struct Options { GAME_MATCH = 0, SHOW_CONTROLS = 1, TITLE_SCREEN = 2, UNKNOWN = 3 };

    std::map<Options, int> optionToPosition = {
        {Options::GAME_MATCH, BULLET_GAME_MATCH_OPTION_Y},
        {Options::SHOW_CONTROLS, BULLET_SHOW_CONTROLS_OPTION_Y},
        {Options::TITLE_SCREEN, BULLET_TITLE_SCREEN_OPTION_Y}};

    std::map<Options, SceneSelection> optionsToScene = {
        {Options::GAME_MATCH, SceneSelection::GAME_MATCH},
        {Options::SHOW_CONTROLS, SceneSelection::SHOW_CONTROLS},
        {Options::TITLE_SCREEN, SceneSelection::TITLE_SCREEN}};

    Options optionsArray[] = {Options::GAME_MATCH, Options::SHOW_CONTROLS, Options::TITLE_SCREEN};
    size_t maxOptionsIndex = sizeof(optionsArray) / sizeof(Options);
    size_t selectedOptionIndex = 0;
    Options selectedOption{};

    SDL_Rect blitPosition;
    blitPosition.x = BULLET_OPTION_X;
    blitPosition.y = BULLET_GAME_MATCH_OPTION_Y;
    blitPosition.w = 12;
    blitPosition.h = 12;
    while (!quitScene) {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (selectedOptionIndex == 0) {
                            selectedOptionIndex = maxOptionsIndex - 1;
                        } else {
                            selectedOptionIndex--;
                        }

                        selectedOption = optionsArray[selectedOptionIndex];
                        blitPosition.y = optionToPosition[selectedOption];
                        break;
                    case SDLK_DOWN:

                        if (selectedOptionIndex == maxOptionsIndex - 1) {
                            selectedOptionIndex = 0;
                        } else {
                            selectedOptionIndex++;
                        }

                        selectedOption = optionsArray[selectedOptionIndex];
                        blitPosition.y = optionToPosition[selectedOption];
                        break;
                    case SDLK_RETURN:
                        quitScene = true;
                        selectedOption = optionsArray[selectedOptionIndex];
                        selectedScene = optionsToScene[selectedOption];
                        break;
                    default:
                        break;
                }
            }
            if (event.type == SDL_QUIT) {
                quitScene = true;
                quitApp = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, titleScreenTexture, NULL, NULL);

        renderText(renderer, fontArmaliteRifle50, "Campo de Batalha", 80, 60, blackColor);

        renderText(renderer, fontArmaliteRifle24, "Start Match", GAME_MATCH_OPTION_X,
                   GAME_MATCH_OPTION_Y, blackColor);
        renderText(renderer, fontArmaliteRifle24, "Show game controls", SHOW_CONTROLS_OPTION_X,
                   SHOW_CONTROLS_OPTION_Y, blackColor);
        renderText(renderer, fontArmaliteRifle24, "Back to title Screen", TITLE_SCREEN_OPTION_X,
                   TITLE_SCREEN_OPTION_Y, blackColor);

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

void runShowControlsScene(SDL_Renderer* renderer, SDL_Event& event,
                          SDL_Texture* showControlsTexture, SceneSelection& selectedScene,
                          int& winnerIdNumber, bool& quitApp, TTF_Font* fontArmaliteRifle18,
                          TTF_Font* fontArmaliteRifle24, SDL_Color blackColor) {
    Uint32 pressEnterCounter = 0;

    bool quitScene = false;
    bool firstFrame = true;
    while (!quitScene) {
        while (SDL_PollEvent(&event)) {
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
        SDL_RenderCopy(renderer, showControlsTexture, NULL, NULL);

        renderText(renderer, fontArmaliteRifle24, "Player 1 controls:", 80, 40, blackColor);

        renderText(renderer, fontArmaliteRifle18, "Arrow UP -> Move Up", 100, 70, blackColor);
        renderText(renderer, fontArmaliteRifle18, "Arrow LEFT -> Move Left", 100, 95, blackColor);
        renderText(renderer, fontArmaliteRifle18, "Arrow DOWN -> Move Down", 100, 120, blackColor);
        renderText(renderer, fontArmaliteRifle18, "Arrow RIGHT -> Move Right", 100, 145,
                   blackColor);
        renderText(renderer, fontArmaliteRifle18, "M -> Fire Bullet", 100, 170, blackColor);

        renderText(renderer, fontArmaliteRifle24, "Player 2 controls:", 80, 200, blackColor);

        renderText(renderer, fontArmaliteRifle18, "W -> Move Up", 100, 230, blackColor);
        renderText(renderer, fontArmaliteRifle18, "A -> Move Left", 100, 255, blackColor);
        renderText(renderer, fontArmaliteRifle18, "S -> Move Down", 100, 280, blackColor);
        renderText(renderer, fontArmaliteRifle18, "D -> Move Right", 100, 305, blackColor);
        renderText(renderer, fontArmaliteRifle18, "F -> Fire Bullet", 100, 330, blackColor);

        if (pressEnterCounter >= 0 && pressEnterCounter < 50) {
            renderText(renderer, fontArmaliteRifle24, "Press any key!", 240, 360, blackColor);
        }
        pressEnterCounter = (pressEnterCounter >= 75) ? 0 : pressEnterCounter + 1;

        SDL_RenderPresent(renderer);

        if (firstFrame) {
            SDL_Delay(500);
            firstFrame = false;
        } else {
            SDL_Delay(16);
        }
    }
}

void setExplosionFrame(Player& player, Explosion& explosion) {
    if (player.explodingTimer > 25) {
        explosion.frame = 0;
    } else if (player.explodingTimer > 20) {
        explosion.frame = 1;
    } else if (player.explodingTimer > 15) {
        explosion.frame = 2;
    } else if (player.explodingTimer > 10) {
        explosion.frame = 3;
    } else {
        explosion.frame = 2;
    }
}

void runGameMatch(SDL_Renderer* renderer, SDL_Event& event, TTF_Font* fontArmaliteRifle32,
                  Player& player1, Player& player2, SDL_Texture* projectileTexture,
                  SDL_Texture* explosionTexture, SDL_Texture* backgroundTexture,
                  SceneSelection& selectedScene, int& winnerIdNumber, bool& quitApp,
                  SDL_Color blackColor, Explosion& explosion1, Explosion& explosion2) {
    std::vector<Projectile> projectiles{};
    size_t projectiles_counter = 0;

    int scorePlayer1 = 0, scorePlayer2 = 0;

    player1.resetStates();
    player2.resetStates();

    SDL_Rect wall;
    SDL_Rect menu;
    SDL_Rect scorePlayer1Position;
    SDL_Rect scorePlayer2Position;

    const int PLAYER_ONE_TEXT_X = 20;
    const int PLAYER_ONE_TEXT_Y = 430;

    const int PLAYER_TWO_TEXT_X = 340;
    const int PLAYER_TWO_TEXT_Y = 430;

    scorePlayer1Position.x = 280, scorePlayer1Position.y = 430;
    scorePlayer2Position.x = 600, scorePlayer2Position.y = 430;

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
        const SDL_Rect player1CollisionBox = createCollisionBox(
            player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
        const SDL_Rect player2CollisionBox = createCollisionBox(
            player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
        switch (lastPressedKeyPlayer1) {
            case SDLK_UP: {
                SDL_Rect movingToCollisionBox = createCollisionBox(
                    player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.y = movingToCollisionBox.y - PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu,
                                   projectiles, false, false)) {
                    player1.tryWalkUp();
                }
                break;
            }
            case SDLK_DOWN: {
                SDL_Rect movingToCollisionBox = createCollisionBox(
                    player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.y = movingToCollisionBox.y + PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu,
                                   projectiles, false, false)) {
                    player1.tryWalkDown();
                }
                break;
            }
            case SDLK_RIGHT: {
                SDL_Rect movingToCollisionBox = createCollisionBox(
                    player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.x = movingToCollisionBox.x + PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu,
                                   projectiles, false, false)) {
                    player1.tryWalkRight();
                }
                break;
            }
            case SDLK_LEFT: {
                SDL_Rect movingToCollisionBox = createCollisionBox(
                    player1.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.x = movingToCollisionBox.x - PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player2CollisionBox, wall, menu,
                                   projectiles, false, false)) {
                    player1.tryWalkLeft();
                }
                break;
            }
            case SDLK_m: {
                fireProjectile(player1, projectiles, projectiles_counter);
            } break;
            default:
                break;
        }

        lastPressedKeyPlayer2 = getLastPressedKey(pressedKeysPlayer2);
        switch (lastPressedKeyPlayer2) {
            case SDLK_w: {
                SDL_Rect movingToCollisionBox = createCollisionBox(
                    player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.y = movingToCollisionBox.y - PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu,
                                   projectiles, false, false)) {
                    player2.tryWalkUp();
                }
                break;
            }
            case SDLK_s: {
                SDL_Rect movingToCollisionBox = createCollisionBox(
                    player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.y = movingToCollisionBox.y + PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu,
                                   projectiles, false, false)) {
                    player2.tryWalkDown();
                }
                break;
            }
            case SDLK_d: {
                SDL_Rect movingToCollisionBox = createCollisionBox(
                    player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.x = movingToCollisionBox.x + PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu,
                                   projectiles, false, false)) {
                    player2.tryWalkRight();
                }
                break;
            }
            case SDLK_a: {
                SDL_Rect movingToCollisionBox = createCollisionBox(
                    player2.position, PLAYER_COLLISION_BOX_SHRINK, PLAYER_COLLISION_BOX_SHRINK);
                movingToCollisionBox.x = movingToCollisionBox.x - PLAYER_MOVING_UNITS;
                if (checkCollision(movingToCollisionBox, player1CollisionBox, wall, menu,
                                   projectiles, false, false)) {
                    player2.tryWalkLeft();
                }
                break;
            }
            case SDLK_f: {
                fireProjectile(player2, projectiles, projectiles_counter);
            } break;
            default:
                break;
        }

        if (player1.explodingState != PlayerStateEnum::EXPLODING &&
            !checkCollision(player1CollisionBox, player2CollisionBox, wall, menu, projectiles, true,
                            true)) {
            scorePlayer2++;
            player1.explodingState = PlayerStateEnum::EXPLODING;
            player1.explodingTimer = PLAYER_EXPLODING_TIMER;
        }

        if (player2.explodingState != PlayerStateEnum::EXPLODING &&
            !checkCollision(player2CollisionBox, player1CollisionBox, wall, menu, projectiles, true,
                            true)) {
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

        drawPlayer(player1.direction, player1.position, player1.spriteTexture, player1.spriteSet,
                   renderer);

        if (player1.explodingState == PlayerStateEnum::EXPLODING) {
            setExplosionFrame(player1, explosion1);
            drawExplosion(explosion1.frame, player1.position, explosion1.spriteTexture,
                          explosion1.spriteSet, renderer);
            // SDL_RenderCopy(renderer, explosionTexture, NULL, &player1.position);
        }

        drawPlayer(player2.direction, player2.position, player2.spriteTexture, player2.spriteSet,
                   renderer);

        if (player2.explodingState == PlayerStateEnum::EXPLODING) {
            setExplosionFrame(player2, explosion2);
            drawExplosion(explosion2.frame, player2.position, explosion2.spriteTexture,
                          explosion2.spriteSet, renderer);
        }

        for (auto projectile = projectiles.begin(); projectile != projectiles.end(); ++projectile) {
            SDL_Rect blitPosition = projectile->position;
            SDL_RenderCopy(renderer, projectileTexture, NULL, &blitPosition);
        }

        renderText(renderer, fontArmaliteRifle32, "Player 1", PLAYER_ONE_TEXT_X, PLAYER_ONE_TEXT_Y,
                   blackColor);
        renderInteger(renderer, fontArmaliteRifle32, scorePlayer1, scorePlayer1Position.x,
                      scorePlayer1Position.y, blackColor);

        renderText(renderer, fontArmaliteRifle32, "Player 2", PLAYER_TWO_TEXT_X, PLAYER_TWO_TEXT_Y,
                   blackColor);
        renderInteger(renderer, fontArmaliteRifle32, scorePlayer2, scorePlayer2Position.x,
                      scorePlayer2Position.y, blackColor);

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
            winnerIdNumber = 1;
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

void runWinnerScene(SDL_Renderer* renderer, SDL_Event& event, SDL_Texture* winnerScreenTexture,
                    SceneSelection& selectedScene, int& winnerIdNumber, bool& quitApp,
                    TTF_Font* fontArmaliteRifle24, SDL_Color blackColor) {
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

    SDL_Init(SDL_INIT_TIMER);  // inicializa��o das fun��es de tempo

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

    Explosion explosion1{"Player1"};
    Explosion explosion2{"Player2"};

    SDL_Event event;  // variavel para determina��o de eventos

    int winnerIdNumber;
    bool quitApp = false, quitScene = false;

    SceneSelection selectedScene = SceneSelection::TITLE_SCREEN;

    SDL_Window* window = SDL_CreateWindow("Campo de Batalha", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    player1.loadSprite("resources/jogador1.bmp", renderer);
    player2.loadSprite("resources/jogador2.bmp", renderer);
    explosion1.loadSprite("resources/explosao.bmp", renderer);
    explosion2.loadSprite("resources/explosao.bmp", renderer);
    SDL_Texture* projectileTexture =
        createTextureFromBMPWithGreenBG(renderer, "resources/bala.bmp");
    SDL_Texture* explosionTexture =
        createTextureFromBMPWithGreenBG(renderer, "resources/explosao.bmp");
    SDL_Texture* titleScreenTexture = createTextureFromBMP(renderer, "resources/telainicial.bmp");
    SDL_Texture* showControlsTexture =
        createTextureFromBMP(renderer, "resources/show_controls_screen.bmp");
    SDL_Texture* backgroundTexture = createTextureFromBMP(renderer, "resources/fundo.bmp");
    SDL_Texture* winnerScreenTexture = createTextureFromBMP(renderer, "resources/telavencedor.bmp");

    // Load a font
    const std::string fontFileName = "resources/armalite_rifle.ttf";
    TTF_Font* fontArmaliteRifle18 = TTF_OpenFont(fontFileName.c_str(), 18);
    if (!fontArmaliteRifle18) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* fontArmaliteRifle24 = TTF_OpenFont(fontFileName.c_str(), 24);
    if (!fontArmaliteRifle24) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* fontArmaliteRifle32 = TTF_OpenFont(fontFileName.c_str(), 32);
    if (!fontArmaliteRifle32) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* fontArmaliteRifle50 = TTF_OpenFont(fontFileName.c_str(), 50);
    if (!fontArmaliteRifle50) {
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
    const char* filenames[3] = {"resources/songtrack001.wav", "resources/songtrack002.wav",
                                "resources/songtrack003.wav"};

    for (int i = 0; i < 3; ++i) {
        if (SDL_LoadWAV(filenames[i], &wavSpec, &tracks[i].start, &tracks[i].length) == NULL) {
            std::cerr << "Failed to load WAV: " << filenames[i] << " Error: " << SDL_GetError()
                      << std::endl;
            SDL_Quit();
            return -1;
        }
    }

    AudioData audioData;
    audioData.tracks = tracks;
    audioData.numTracks = 3;
    audioData.currentTrack = 0;  // Start with track 0
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
        if (selectedScene == SceneSelection::TITLE_SCREEN) {
            runTitleScreen(titleScreenTexture, renderer, event, quitApp, selectedScene,
                           fontArmaliteRifle24, fontArmaliteRifle50, whiteColor, blackColor);
        } else if (selectedScene == SceneSelection::MAIN_MENU) {
            runMainMenu(renderer, event, quitApp, selectedScene, titleScreenTexture,
                        projectileTexture, fontArmaliteRifle24, fontArmaliteRifle50, whiteColor,
                        blackColor);
        } else if (selectedScene == SceneSelection::SHOW_CONTROLS) {
            runShowControlsScene(renderer, event, showControlsTexture, selectedScene,
                                 winnerIdNumber, quitApp, fontArmaliteRifle18, fontArmaliteRifle24,
                                 blackColor);
        } else if (selectedScene == SceneSelection::GAME_MATCH)  // condicao 2 (o jogo em si)
        {
            runGameMatch(renderer, event, fontArmaliteRifle32, player1, player2, projectileTexture,
                         explosionTexture, backgroundTexture, selectedScene, winnerIdNumber,
                         quitApp, blackColor, explosion1, explosion2);
        } else if (selectedScene == SceneSelection::WINNER_SCREEN) {
            runWinnerScene(renderer, event, winnerScreenTexture, selectedScene, winnerIdNumber,
                           quitApp, fontArmaliteRifle24, blackColor);

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
