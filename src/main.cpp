#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>

#include <iostream>
#include <vector>

#define SDL_MAIN_HANDLED  // Add this line

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "Constants.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "SpriteSet.hpp"

// funcao para carregar os tanques na tela
void jogador_load(int playerPosition, SDL_Rect offset2, SDL_Surface* jogador2,
                  SpriteSet& playerSpriteSet, SDL_Surface* screen) {
    SDL_Rect playerSpriteRect;
    switch (playerPosition) {
        case static_cast<int>(DirectionEnum::UP):
            playerSpriteRect = playerSpriteSet.getRect("UP");
            SDL_BlitSurface(jogador2, &playerSpriteRect, screen, &offset2);
            break;
        case static_cast<int>(DirectionEnum::DOWN):
            playerSpriteRect = playerSpriteSet.getRect("DOWN");
            SDL_BlitSurface(jogador2, &playerSpriteRect, screen, &offset2);
            break;
        case static_cast<int>(DirectionEnum::RIGHT):
            playerSpriteRect = playerSpriteSet.getRect("RIGHT");
            SDL_BlitSurface(jogador2, &playerSpriteRect, screen, &offset2);
            break;
        case static_cast<int>(DirectionEnum::LEFT):
            playerSpriteRect = playerSpriteSet.getRect("LEFT");
            SDL_BlitSurface(jogador2, &playerSpriteRect, screen, &offset2);
            break;
    }
}

// funcao para carregar as pontuacoes dos jogadores na tela
void pontuacao_load(int pontuacao3, SDL_Rect pontuacao03, SDL_Surface* pontuacao,
                    SpriteSet& pontuacao01, SDL_Surface* screen) {
    SDL_Rect pontuacaoSpriteRect;
    switch (pontuacao3) {
        case 0:
            pontuacaoSpriteRect = pontuacao01.getRect("ZERO");
            SDL_BlitSurface(pontuacao, &pontuacaoSpriteRect, screen, &pontuacao03);
            break;
        case 1:
            pontuacaoSpriteRect = pontuacao01.getRect("ONE");
            SDL_BlitSurface(pontuacao, &pontuacaoSpriteRect, screen, &pontuacao03);
            break;
        case 2:
            pontuacaoSpriteRect = pontuacao01.getRect("TWO");
            SDL_BlitSurface(pontuacao, &pontuacaoSpriteRect, screen, &pontuacao03);
            break;
        case 3:
            pontuacaoSpriteRect = pontuacao01.getRect("THREE");
            SDL_BlitSurface(pontuacao, &pontuacaoSpriteRect, screen, &pontuacao03);
            break;
        case 4:
            pontuacaoSpriteRect = pontuacao01.getRect("FOUR");
            SDL_BlitSurface(pontuacao, &pontuacaoSpriteRect, screen, &pontuacao03);
            break;
        case 5:
            pontuacaoSpriteRect = pontuacao01.getRect("FIVE");
            SDL_BlitSurface(pontuacao, &pontuacaoSpriteRect, screen, &pontuacao03);
            break;
        case 6:
            pontuacaoSpriteRect = pontuacao01.getRect("SIX");
            SDL_BlitSurface(pontuacao, &pontuacaoSpriteRect, screen, &pontuacao03);
            break;
    }
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

bool checkCollision(SDL_Rect& firstPlayerPosition, SDL_Rect& secondPlayerPosition, SDL_Rect& wall,
                    SDL_Rect& menu, std::vector<Projectile>& projectiles) {
    bool notCollidedWithProjectile = true;
    for (auto projectile = projectiles.begin(); projectile != projectiles.end();) {
        bool _notCollidedWithProjectile = checkCollisionBetweenTwoRects(
            firstPlayerPosition.x, firstPlayerPosition.y, firstPlayerPosition.w,
            firstPlayerPosition.h, projectile->position.x, projectile->position.y,
            projectile->position.w, projectile->position.h);
        std::cout << "Not collided with projectile: " << _notCollidedWithProjectile << std::endl;
        notCollidedWithProjectile = (notCollidedWithProjectile && _notCollidedWithProjectile);

        if (!_notCollidedWithProjectile) {
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

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);    // inicializa��o das fun��es de video
    SDL_Init(SDL_INIT_TIMER);    // inicializa��o das fun��es de tempo
    SDL_Surface* screen;         // superficie de video
    SDL_Surface* telaprincipal;  // supercie de fundo
    SDL_Surface* bala;           // superficie da bala
    SDL_Surface* explosao;       // superficie da explos�o
    SDL_Surface* pontuacao;      // superficie dos numeros para pontua��o

    SpriteSet pontuacao01{"Pontuacao01"};
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
        vencedor0;  // variaveis de controle e de pontuacao e numeracao

    quit1 = 0;
    quit2 = 0;
    entrar = 1;
    SDL_EnableKeyRepeat(50, 50);  // habilitar a repeti��o de teclas

    while (quit1 == 0)  // loop principal
    {
        screen =
            SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);  // atribui��o de uma superficie de video
                                                            // 640 x 480 para a variavel screen

        if (entrar == 1)  // condi��o 1 (para tela inicial)
        {
            // atribui��o de imagens bmp para as superficies do jogo
            telaprincipal = SDL_LoadBMP("resources/telainicial.bmp");
            // jogador1 = SDL_LoadBMP("resources/jogador1.bmp");
            player1.loadSprite("resources/jogador1.bmp");
            player2.loadSprite("resources/jogador2.bmp");

            bala = SDL_LoadBMP("resources/bala.bmp");
            SDL_SetColorKey(
                bala, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                (Uint16)SDL_MapRGB(bala->format, 0, 255, 0));  // para por transparencia no verde.

            explosao = SDL_LoadBMP("resources/explosao.bmp");
            SDL_SetColorKey(explosao, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                            (Uint16)SDL_MapRGB(explosao->format, 0, 255,
                                               0));  // para por transparencia no verde.
            pontuacao = SDL_LoadBMP("resources/numeros.bmp");
            SDL_SetColorKey(pontuacao, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                            (Uint16)SDL_MapRGB(pontuacao->format, 0, 255,
                                               0));              // para por transparencia no verde.
            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);  // carrega a telaprincipal no video
            SDL_Flip(screen);

            if (SDL_PollEvent(&event))  // condi��o de entrada (enter)
            {
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_RETURN:
                            quit2 = 0;
                            entrar = 3;
                        default:
                            x = 1;
                    }
                }
                if (event.type == SDL_QUIT)  // para fechar o programa clicando no X
                {
                    quit1 = 1;
                }
            }
        } else if (entrar == 3)  // condi��o de entrada no menu
        {
            SDL_FreeSurface(telaprincipal);
            telaprincipal = SDL_LoadBMP("resources/menu.bmp");  // troca a imagem bmp para o menu
            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
            SDL_Flip(screen);

            if (SDL_PollEvent(&event))  // menu do jogo.
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
                            x = 1;
                    }
                }
                if (event.type == SDL_QUIT)  // fechar o programa caso cliquem no X
                {
                    quit1 = 1;
                }
            }
        } else if (entrar == 2)  // condicao 2 (o jogo em si)
        {
            // atribuicao de variaveis inteiras , retangulos , coordenadas e etc.
            pontuacao1 = 0;
            pontuacao2 = 0;

            SDL_FreeSurface(telaprincipal);
            telaprincipal = SDL_LoadBMP("resources/fundo.bmp");
            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);

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
            pontuacao01.addRect("ZERO", 0, 3, 25, 35);
            pontuacao01.addRect("ONE", 50, 3, 25, 35);
            pontuacao01.addRect("TWO", 98, 3, 25, 35);
            pontuacao01.addRect("THREE", 147, 3, 25, 35);
            pontuacao01.addRect("FOUR", 195, 3, 25, 35);
            pontuacao01.addRect("FIVE", 245, 3, 25, 35);
            pontuacao01.addRect("SIX", 294, 3, 25, 35);

            SDL_Rect pontuacaoZeroRect = pontuacao01.getRect("ZERO");
            SDL_BlitSurface(pontuacao, &pontuacaoZeroRect, screen, &pontuacao02);
            SDL_BlitSurface(pontuacao, &pontuacaoZeroRect, screen, &pontuacao03);

            SDL_Rect playerSpriteRect;

            player1.setDirection(DirectionEnum::RIGHT);
            playerSpriteRect = player1.spriteSet.getRect("RIGHT");
            SDL_BlitSurface(player1.spriteSurface, &playerSpriteRect, screen, &player1.position);

            player2.setDirection(DirectionEnum::LEFT);
            playerSpriteRect = player2.spriteSet.getRect("LEFT");
            SDL_BlitSurface(player2.spriteSurface, &playerSpriteRect, screen, &player2.position);
            SDL_Flip(screen);

            while (quit2 == 0)  // loop para controlar o fim do jogo
            {
                while (SDL_PollEvent(&event))  // loop de eventos
                {
                    switch (event.type) {
                        case SDL_KEYDOWN:
                            switch (event.key.keysym.sym) {  // comandos para o jogador 1
                                case SDLK_UP:                // tecla para cima
                                    if (player1.state == PlayerStateEnum::IDLE) {
                                        if (checkCollision(player1.position, player2.position, wall,
                                                           menu, projectiles)) {
                                            player1.position.y = player1.position.y - 6;
                                            player1.setDirection(DirectionEnum::UP);
                                            break;
                                        }
                                    }
                                    break;
                                case SDLK_DOWN:  // tecla para baixo
                                    if (player1.state == PlayerStateEnum::IDLE) {
                                        if (checkCollision(player1.position, player2.position, wall,
                                                           menu, projectiles)) {
                                            // Update position and direction of player1
                                            player1.position.y = player1.position.y + 6;
                                            player1.setDirection(DirectionEnum::DOWN);
                                            break;
                                        }
                                    }
                                    break;
                                case SDLK_RIGHT:  // tecla para a direita
                                    if (player1.state == PlayerStateEnum::IDLE) {
                                        if (checkCollision(player1.position, player2.position, wall,
                                                           menu, projectiles)) {
                                            // Update position and direction of player1
                                            player1.position.x = player1.position.x + 6;
                                            player1.setDirection(DirectionEnum::RIGHT);
                                            break;
                                        }
                                    }
                                    break;
                                case SDLK_LEFT:  // tecla para esquerda
                                    if (player1.state == PlayerStateEnum::IDLE) {
                                        if (checkCollision(player1.position, player2.position, wall,
                                                           menu, projectiles)) {
                                            // Update position and direction of player1
                                            player1.position.x = player1.position.x - 6;
                                            player1.setDirection(DirectionEnum::LEFT);
                                            break;
                                        }
                                    }
                                    break;
                                // comandos para o jogador 2
                                case SDLK_w:  // tecla w ( para cima )
                                    if (player2.state == PlayerStateEnum::IDLE) {
                                        if (checkCollision(player2.position, player1.position, wall,
                                                           menu, projectiles)) {
                                            // Update position and direction of player2
                                            player2.position.y = player2.position.y - 6;
                                            player2.setDirection(DirectionEnum::UP);
                                            break;
                                        }
                                    }
                                    break;
                                case SDLK_s:  // tecla s ( para baixo )
                                    if (player2.state == PlayerStateEnum::IDLE) {
                                        if (checkCollision(player2.position, player1.position, wall,
                                                           menu, projectiles)) {
                                            // Update position and direction of player2
                                            player2.position.y = player2.position.y + 6;
                                            player2.setDirection(DirectionEnum::DOWN);
                                            break;
                                        }
                                    }
                                    break;
                                case SDLK_d:  // tecla d ( para direita )
                                    if (player2.state == PlayerStateEnum::IDLE) {
                                        if (checkCollision(player2.position, player1.position, wall,
                                                           menu, projectiles)) {
                                            // Update position and direction of player2
                                            player2.position.x = player2.position.x + 6;
                                            player2.setDirection(DirectionEnum::RIGHT);
                                            break;
                                        }
                                    }
                                    break;
                                case SDLK_a:  // tecla a ( para esquerda )
                                    if (player2.state == PlayerStateEnum::IDLE) {
                                        if (checkCollision(player2.position, player1.position, wall,
                                                           menu, projectiles)) {
                                            // Update position and direction of player2
                                            player2.position.x = player2.position.x - 6;
                                            player2.setDirection(DirectionEnum::LEFT);
                                            break;
                                        }
                                    }
                                    break;
                                case SDLK_ESCAPE:
                                    quit2 = 1;
                                    break;
                                // comandos para o tiro dos jogadores 1 e 2
                                case SDLK_m:  // tecla m para o tiro do jogador 1
                                {
                                    Projectile projectile = Projectile(projectiles_counter);
                                    projectile.loadSprite("resources/bala.bmp");

                                    switch (player1.direction) {
                                        case static_cast<int>(DirectionEnum::UP): {
                                            projectile.position.x = player1.position.x + 12;
                                            projectile.position.y = player1.position.y - 15;
                                            projectile.position.w = 12;
                                            projectile.position.h = 12;
                                            projectile.setDirection(DirectionEnum::UP);
                                        } break;
                                        case static_cast<int>(DirectionEnum::DOWN): {
                                            projectile.position.x = player1.position.x + 12;
                                            projectile.position.y = player1.position.y + 36;
                                            projectile.position.w = 12;
                                            projectile.position.h = 12;
                                            projectile.setDirection(DirectionEnum::DOWN);
                                        } break;
                                        case static_cast<int>(DirectionEnum::RIGHT): {
                                            projectile.position.x = player1.position.x + 36;
                                            projectile.position.y = player1.position.y + 12;
                                            projectile.position.w = 12;
                                            projectile.position.h = 12;
                                            projectile.setDirection(DirectionEnum::RIGHT);
                                        } break;
                                        case static_cast<int>(DirectionEnum::LEFT): {
                                            projectile.position.x = player1.position.x - 15;
                                            projectile.position.y = player1.position.y + 12;
                                            projectile.position.w = 12;
                                            projectile.position.h = 12;
                                            projectile.setDirection(DirectionEnum::LEFT);
                                        } break;

                                        default:
                                            break;
                                    }
                                    projectile.movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                                    projectiles.push_back(projectile);
                                    projectiles_counter++;
                                } break;
                                case SDLK_f:  // tecla f para o jogador 2
                                {
                                    Projectile projectile = Projectile(projectiles_counter);
                                    projectile.loadSprite("resources/bala.bmp");

                                    switch (player2.direction) {
                                        case static_cast<int>(DirectionEnum::UP): {
                                            projectile.position.x = player2.position.x + 12;
                                            projectile.position.y = player2.position.y - 15;
                                            projectile.position.w = 12;
                                            projectile.position.h = 12;
                                            projectile.setDirection(DirectionEnum::UP);
                                        } break;
                                        case static_cast<int>(DirectionEnum::DOWN): {
                                            projectile.position.x = player2.position.x + 12;
                                            projectile.position.y = player2.position.y + 36;
                                            projectile.position.w = 12;
                                            projectile.position.h = 12;
                                            projectile.setDirection(DirectionEnum::DOWN);
                                        } break;
                                        case static_cast<int>(DirectionEnum::RIGHT): {
                                            projectile.position.x = player2.position.x + 36;
                                            projectile.position.y = player2.position.y + 12;
                                            projectile.position.w = 12;
                                            projectile.position.h = 12;
                                            projectile.setDirection(DirectionEnum::RIGHT);
                                        } break;
                                        case static_cast<int>(DirectionEnum::LEFT): {
                                            projectile.position.x = player2.position.x - 15;
                                            projectile.position.y = player2.position.y + 12;
                                            projectile.position.w = 12;
                                            projectile.position.h = 12;
                                            projectile.setDirection(DirectionEnum::LEFT);
                                        } break;
                                        default:
                                            break;
                                    }
                                    projectile.movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                                    projectiles.push_back(projectile);
                                    projectiles_counter++;
                                } break;
                                default:
                                    x = 0;
                            }
                        case SDL_KEYUP:
                            switch (event.key.keysym.sym) {
                                default:
                                    x = 1;
                                    break;
                            }
                    }
                    if (event.type == SDL_QUIT)  // fechar o programa caso cliquem no X
                    {
                        quit1 = 1;
                        quit2 = 1;
                    }
                }

                if (player1.state != PlayerStateEnum::EXPLODING &&
                    !checkCollision(player1.position, player2.position, wall, menu,
                                           projectiles)) {
                    pontuacao2 = pontuacao2 + 1;
                    player1.state = PlayerStateEnum::EXPLODING;
                    player1.timer = 1000;
                }

                if (player2.state != PlayerStateEnum::EXPLODING &&
                    !checkCollision(player2.position, player1.position, wall, menu,
                                           projectiles)) {
                    pontuacao1 = pontuacao1 + 1;
                    player2.state = PlayerStateEnum::EXPLODING;
                    player2.timer = 1000;
                }

                // Here it should be the drawing logic!

                // Define the color to clear the screen (e.g., black)
                Uint32 clearColor = SDL_MapRGB(screen->format, 0, 0, 0);

                // Fill the screen surface with the clear color
                SDL_FillRect(screen, NULL, clearColor);

                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);

                jogador_load(player1.direction, player1.position, player1.spriteSurface,
                             player1.spriteSet, screen);

                switch (player1.state) {
                    case PlayerStateEnum::EXPLODING:
                        if (player1.timer > 0) {
                            SDL_BlitSurface(explosao, NULL, screen, &player1.position);
                            player1.timer--;
                        } else {
                            player1.state = PlayerStateEnum::IDLE;
                            player1.position.x = 15, player1.position.y = 170;
                        }

                        break;

                    default:
                        break;
                }

                jogador_load(player2.direction, player2.position, player2.spriteSurface,
                             player2.spriteSet, screen);

                switch (player2.state) {
                    case PlayerStateEnum::EXPLODING: {
                        if (player2.timer > 0) {
                            SDL_BlitSurface(explosao, NULL, screen, &player2.position);
                            player2.timer--;
                        } else {
                            player2.state = PlayerStateEnum::IDLE;
                            player2.position.x = 560, player2.position.y = 260;
                        }
                    } break;

                    default:
                        break;
                }

                for (auto projectile = projectiles.begin(); projectile != projectiles.end();) {
                    SDL_Rect blitPosition = projectile->position;
                    SDL_BlitSurface(projectile->spriteSurface, NULL, screen, &blitPosition);

                    if (projectile->movingCooldown > 0) {
                        projectile->movingCooldown--;
                    } else {
                        switch (projectile->direction) {
                            case static_cast<int>(DirectionEnum::UP): {
                                projectile->position.y--;
                                projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                            } break;
                            case static_cast<int>(DirectionEnum::DOWN): {
                                projectile->position.y++;
                                projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                            } break;
                            case static_cast<int>(DirectionEnum::RIGHT): {
                                projectile->position.x++;
                                projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                            } break;
                            case static_cast<int>(DirectionEnum::LEFT): {
                                projectile->position.x--;
                                projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                            } break;

                            default:
                                break;
                        }
                    }
                    if (projectile->position.x < -10 || projectile->position.x > 1000 ||
                        projectile->position.y < -10 || projectile->position.y > 1000) {
                        std::cout << "Before erase projectile" << std::endl;
                        projectile =
                            projectiles.erase(projectile);  // erase returns the next valid iterator
                    } else {
                        ++projectile;  // Only increment if no deletion occurred
                    }
                }

                // Get the length of the vector
                std::size_t length = projectiles.size();

                std::cout << "The length of the vector is: " << length << std::endl;

                pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                SDL_Flip(screen);

                // condi��es para verificar a vitoria dos jogadores
                if (pontuacao1 == 6)  // verificar vitoria do jogador 1
                {
                    vencedor0 = 1;
                    SDL_Delay(1000);
                    quit2 = 1;
                    SDL_FreeSurface(telaprincipal);
                    telaprincipal = SDL_LoadBMP("resources/telavencedor.bmp");
                    SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                    pontuacao_load(vencedor0, vencedor, pontuacao, pontuacao01, screen);
                    SDL_Flip(screen);
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
                    vencedor0 = 2;
                    SDL_Delay(1000);
                    quit2 = 1;
                    SDL_FreeSurface(telaprincipal);
                    telaprincipal = SDL_LoadBMP("resources/telavencedor.bmp");
                    SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                    pontuacao_load(vencedor0, vencedor, pontuacao, pontuacao01, screen);
                    SDL_Flip(screen);
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
