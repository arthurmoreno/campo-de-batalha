#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>

#define SDL_MAIN_HANDLED  // Add this line

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

SDL_Rect offset;         // coordenada do jogador 1
SDL_Rect offset2;        // coordenada do jogador 2
int posicao1, posicao2;  // variavel para determinacaoo das posicoes dos jogadores

// funcao para carregar os tanques na tela
void jogador_load(int posicao2, SDL_Rect offset2, SDL_Surface *jogador2, SDL_Rect jogador2_c[],
                  SDL_Surface *screen) {
    switch (posicao2) {
        case 0:
            SDL_BlitSurface(jogador2, &jogador2_c[1], screen, &offset2);
            break;
        case 1:
            SDL_BlitSurface(jogador2, &jogador2_c[2], screen, &offset2);
            break;
        case 2:
            SDL_BlitSurface(jogador2, &jogador2_c[0], screen, &offset2);
            break;
        case 3:
            SDL_BlitSurface(jogador2, &jogador2_c[3], screen, &offset2);
            break;
    }
}

// funcao para carregar as pontuacoes dos jogadores na tela
void pontuacao_load(int pontuacao3, SDL_Rect pontuacao03, SDL_Surface *pontuacao,
                    SDL_Rect pontuacao01[], SDL_Surface *screen) {
    switch (pontuacao3) {
        case 0:
            SDL_BlitSurface(pontuacao, &pontuacao01[0], screen, &pontuacao03);
            break;
        case 1:
            SDL_BlitSurface(pontuacao, &pontuacao01[1], screen, &pontuacao03);
            break;
        case 2:
            SDL_BlitSurface(pontuacao, &pontuacao01[2], screen, &pontuacao03);
            break;
        case 3:
            SDL_BlitSurface(pontuacao, &pontuacao01[3], screen, &pontuacao03);
            break;
        case 4:
            SDL_BlitSurface(pontuacao, &pontuacao01[4], screen, &pontuacao03);
            break;
        case 5:
            SDL_BlitSurface(pontuacao, &pontuacao01[5], screen, &pontuacao03);
            break;
        case 6:
            SDL_BlitSurface(pontuacao, &pontuacao01[6], screen, &pontuacao03);
            break;
    }
}

// funcao que verifica a colisao entre objetos
int colisao(int ax, int ay, int aw, int ah, int cx, int cy, int cw, int ch) {
    int bx, by, dx, dy;
    bx = ax + aw;
    by = ay + ah;
    dx = cx + cw;
    dy = cy + ch;
    return (((ax > dx) || (bx < cx) || (ay > dy) || (by < cy)));
}

// funcao para movimentar os tanques
void andar(SDL_Event event, SDL_Surface *screen, SDL_Surface *telaprincipal, SDL_Surface *jogador1,
           SDL_Surface *jogador2, SDL_Surface *bala, SDL_Surface *explosao, SDL_Surface *pontuacao,
           SDL_Rect pontuacao01[], SDL_Rect jogador1_c[], SDL_Rect jogador2_c[], SDL_Rect wall,
           SDL_Rect menu, SDL_Rect offset3, SDL_Rect pontuacao02, SDL_Rect pontuacao03,
           SDL_Rect bala01, SDL_Rect vencedor, int quit1, int quit2, int x, int entrar, int bala1,
           int bala2, int pontuacao1, int pontuacao2, int vencedor0) {
    if (SDL_PollEvent(&event))  // condi��o de entrada (enter)
    {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {  // comandos para o jogador 1
                case SDLK_UP:
                    if (colisao(offset.x, offset.y, offset.w, offset.h, wall.x, wall.y, wall.w,
                                wall.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x, offset2.y,
                                offset2.w, offset2.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, menu.x, menu.y, menu.w,
                                menu.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset3.x, offset3.y,
                                offset3.w, offset3.h)) {
                        offset.y = offset.y - 6;
                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                        SDL_BlitSurface(jogador1, &jogador1_c[1], screen, &offset);
                        jogador_load(posicao2, offset2, jogador2, jogador2_c, screen);
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_Flip(screen);
                        posicao1 = 0;
                        break;
                    } else {
                        pontuacao2 = pontuacao2 + 1;
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        SDL_BlitSurface(explosao, NULL, screen, &offset);
                        SDL_Flip(screen);
                        SDL_Delay(500);
                        offset.x = 15, offset.y = 170;
                    }
                    break;
                case SDLK_DOWN:
                    if (colisao(offset.x, offset.y, offset.w, offset.h, wall.x, wall.y, wall.w,
                                wall.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x, offset2.y,
                                offset2.w, offset2.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, menu.x, menu.y, menu.w,
                                menu.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset3.x, offset3.y,
                                offset3.w, offset3.h)) {
                        offset.y = offset.y + 6;
                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                        SDL_BlitSurface(jogador1, &jogador1_c[2], screen, &offset);
                        jogador_load(posicao2, offset2, jogador2, jogador2_c, screen);
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_Flip(screen);
                        posicao1 = 1;
                        break;
                    } else {
                        pontuacao2 = pontuacao2 + 1;
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        SDL_BlitSurface(explosao, NULL, screen, &offset);
                        SDL_Flip(screen);
                        SDL_Delay(500);
                        offset.x = 15, offset.y = 170;
                    }
                    break;
                case SDLK_RIGHT:
                    if (colisao(offset.x, offset.y, offset.w, offset.h, wall.x, wall.y, wall.w,
                                wall.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x, offset2.y,
                                offset2.w, offset2.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, menu.x, menu.y, menu.w,
                                menu.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset3.x, offset3.y,
                                offset3.w, offset3.h)) {
                        offset.x = offset.x + 6;
                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                        SDL_BlitSurface(jogador1, &jogador1_c[0], screen, &offset);
                        jogador_load(posicao2, offset2, jogador2, jogador2_c, screen);
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_Flip(screen);
                        posicao1 = 2;
                        break;
                    } else {
                        pontuacao2 = pontuacao2 + 1;
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        SDL_BlitSurface(explosao, NULL, screen, &offset);
                        SDL_Flip(screen);
                        SDL_Delay(500);
                        offset.x = 15, offset.y = 170;
                    }
                    break;
                case SDLK_LEFT:
                    if (colisao(offset.x, offset.y, offset.w, offset.h, wall.x, wall.y, wall.w,
                                wall.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x, offset2.y,
                                offset2.w, offset2.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, menu.x, menu.y, menu.w,
                                menu.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset3.x, offset3.y,
                                offset3.w, offset3.h)) {
                        offset.x = offset.x - 6;
                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                        SDL_BlitSurface(jogador1, &jogador1_c[3], screen, &offset);
                        jogador_load(posicao2, offset2, jogador2, jogador2_c, screen);
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_Flip(screen);
                        posicao1 = 3;
                        break;
                    } else {
                        pontuacao2 = pontuacao2 + 1;
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        SDL_BlitSurface(explosao, NULL, screen, &offset);
                        SDL_Flip(screen);
                        SDL_Delay(500);
                        offset.x = 15, offset.y = 170;
                    }
                    break;
                    // comandos para o jogador 2
                case SDLK_w:
                    if (colisao(offset2.x, offset2.y, offset2.w, offset2.h, wall.x, wall.y, wall.w,
                                wall.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x, offset2.y,
                                offset2.w, offset2.h) &&
                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, menu.x, menu.y, menu.w,
                                menu.h) &&
                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, offset3.x, offset3.y,
                                offset3.w, offset3.h)) {
                        offset2.y = offset2.y - 6;
                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                        SDL_BlitSurface(jogador2, &jogador2_c[1], screen, &offset2);
                        jogador_load(posicao1, offset, jogador1, jogador1_c, screen);
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_Flip(screen);
                        posicao2 = 0;
                        break;
                    } else {
                        pontuacao1 = pontuacao1 + 1;
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_BlitSurface(explosao, NULL, screen, &offset2);
                        SDL_Flip(screen);
                        SDL_Delay(500);
                        offset2.x = 560, offset2.y = 260;
                    }
                    break;
                case SDLK_s:
                    if (colisao(offset2.x, offset2.y, offset2.w, offset2.h, wall.x, wall.y, wall.w,
                                wall.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x, offset2.y,
                                offset2.w, offset2.h) &&
                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, menu.x, menu.y, menu.w,
                                menu.h) &&
                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, offset3.x, offset3.y,
                                offset3.w, offset3.h)) {
                        offset2.y = offset2.y + 6;
                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                        SDL_BlitSurface(jogador2, &jogador2_c[2], screen, &offset2);
                        jogador_load(posicao1, offset, jogador1, jogador1_c, screen);
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_Flip(screen);
                        posicao2 = 1;
                        break;
                    } else {
                        pontuacao1 = pontuacao1 + 1;
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_BlitSurface(explosao, NULL, screen, &offset2);
                        SDL_Flip(screen);
                        SDL_Delay(500);
                        offset2.x = 560, offset2.y = 260;
                    }
                    break;
                case SDLK_d:
                    if (colisao(offset2.x, offset2.y, offset2.w, offset2.h, wall.x, wall.y, wall.w,
                                wall.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x, offset2.y,
                                offset2.w, offset2.h) &&
                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, menu.x, menu.y, menu.w,
                                menu.h) &&
                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, offset3.x, offset3.y,
                                offset3.w, offset3.h)) {
                        offset2.x = offset2.x + 6;
                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                        SDL_BlitSurface(jogador2, &jogador2_c[0], screen, &offset2);
                        jogador_load(posicao1, offset, jogador1, jogador1_c, screen);
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_Flip(screen);
                        posicao2 = 2;
                        break;
                    } else {
                        pontuacao1 = pontuacao1 + 1;
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_BlitSurface(explosao, NULL, screen, &offset2);
                        SDL_Flip(screen);
                        SDL_Delay(500);
                        offset2.x = 560, offset2.y = 260;
                    }
                    break;
                case SDLK_a:
                    if (colisao(offset2.x, offset2.y, offset2.w, offset2.h, wall.x, wall.y, wall.w,
                                wall.h) &&
                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x, offset2.y,
                                offset2.w, offset2.h) &&
                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, menu.x, menu.y, menu.w,
                                menu.h) &&
                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, offset3.x, offset3.y,
                                offset3.w, offset3.h)) {
                        offset2.x = offset2.x - 6;
                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                        SDL_BlitSurface(jogador2, &jogador2_c[3], screen, &offset2);
                        jogador_load(posicao1, offset, jogador1, jogador1_c, screen);
                        pontuacao_load(pontuacao2, pontuacao03, pontuacao, pontuacao01, screen);
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_Flip(screen);
                        posicao2 = 3;
                        break;
                    } else {
                        pontuacao1 = pontuacao1 + 1;
                        pontuacao_load(pontuacao1, pontuacao02, pontuacao, pontuacao01, screen);
                        SDL_BlitSurface(explosao, NULL, screen, &offset2);
                        SDL_Flip(screen);
                        SDL_Delay(500);
                        offset2.x = 560, offset2.y = 260;
                    }
                    break;
                default:
                    x = 0;
                    break;
            }
        }
    }
}

int main(int argc, char *args[]) {
    SDL_Init(SDL_INIT_VIDEO);    // inicializa��o das fun��es de video
    SDL_Init(SDL_INIT_TIMER);    // inicializa��o das fun��es de tempo
    SDL_Surface *screen;         // superficie de video
    SDL_Surface *telaprincipal;  // supercie de fundo
    SDL_Surface *jogador1;       // superficie do jogador 1
    SDL_Surface *jogador2;       // superficie do jogador 2
    SDL_Surface *bala;           // superficie da bala
    SDL_Surface *explosao;       // superficie da explos�o
    SDL_Surface *pontuacao;      // superficie dos numeros para pontua��o
    SDL_Rect pontuacao01[7];     //     retangulos    para recorte de imagens dos numeros
    SDL_Rect jogador1_c[4];  //       "               "               "    das posi��es do jogador 1
    SDL_Rect jogador2_c[4];  //       "               "               "    das posi��es do jogador 2
    SDL_Rect wall;           // retangulo para determinar a colis�o com a parede central
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
    SDL_EnableKeyRepeat(200, 200);  // habilitar a repeti��o de teclas

    while (quit1 == 0)  // loop principal
    {
        screen =
            SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);  // atribui��o de uma superficie de video
                                                            // 640 x 480 para a variavel screen

        if (entrar == 1)  // condi��o 1 (para tela inicial)
        {
            // atribui��o de imagens bmp para as superficies do jogo
            telaprincipal = SDL_LoadBMP("resources/telainicial.bmp");
            jogador1 = SDL_LoadBMP("resources/jogador1.bmp");
            SDL_SetColorKey(jogador1, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                            (Uint16)SDL_MapRGB(jogador1->format, 0, 255,
                                               0));  // para por transparencia no verde.
            jogador2 = SDL_LoadBMP("resources/jogador2.bmp");
            SDL_SetColorKey(jogador2, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                            (Uint16)SDL_MapRGB(jogador2->format, 0, 255,
                                               0));  // para por transparencia no verde.
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
        }
        if (entrar == 3)  // condi��o de entrada no menu
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
        }
        if (entrar == 4) {
            SDL_FreeSurface(telaprincipal);
            telaprincipal = SDL_LoadBMP(
                "resources/menuonline.bmp");  // troca a imagem bmp para o menu para online
            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
            SDL_Flip(screen);

            if (SDL_PollEvent(&event))  // menu do jogo.
            {
                if (event.type == SDL_KEYDOWN)  // evento do tipo tecla pressionada
                {
                    switch (event.key.keysym.sym) {
                        case SDLK_1:  // caso seja servidor
                            entrar = 5;
                            break;
                        case SDLK_2:  // caso seja cliente
                            entrar = 6;
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
        }
        if (entrar == 5)  // condi��o de entrada para o servidor(quem criou o jogo)
        {
            WSADATA t_wsa;                                 // WSADATA structure
            WORD wVers;                                    // version number
            int iError, quit, bytesreceive, bytessend, x;  // error number
            char buffer[200];
            quit = 1;

            SDL_FreeSurface(telaprincipal);
            telaprincipal = SDL_LoadBMP(
                "resources/mododeespera.bmp");  // troca a imagem bmp para o menu para online
            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
            SDL_Flip(screen);

            wVers = MAKEWORD(2, 2);              // Set the version number to 2.2
            iError = WSAStartup(wVers, &t_wsa);  // Start the WSADATA

            SOCKET sServer;
            sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            SOCKADDR_IN sinServer;
            memset(&sinServer, 0, sizeof(sinServer));

            sinServer.sin_family = AF_INET;
            sinServer.sin_addr.s_addr = INADDR_ANY;  // Where to start server?
            sinServer.sin_port = htons(1000);        // Port

            if (bind(sServer, (LPSOCKADDR)&sinServer, sizeof(sinServer)) == SOCKET_ERROR) {
                /* failed at starting server */
                MessageBox(NULL, (LPCTSTR) "Could not bind the server!()",
                           (LPCTSTR) "Campo de batalha", MB_OK | MB_ICONERROR);
                WSACleanup();
                return 0;
            }

            while (listen(sServer, 20) == SOCKET_ERROR) {
                Sleep(10);
            }
            MessageBox(NULL, (LPCTSTR) "Esperando por clientes", (LPCTSTR) "Campo de batalha",
                       MB_OK);

            SOCKET sClient;
            int szlength;

            szlength = sizeof(sinServer);
            sClient = accept(sServer, (LPSOCKADDR)&sinServer, &szlength);

            if (sClient == INVALID_SOCKET) {
                MessageBox(NULL, (LPCTSTR) "Nao eh possivel aceitar esse cliente!",
                           (LPCTSTR) "Campo de batalha", MB_OK);
                closesocket(sServer);
                WSACleanup();
                return 0;
            } else {
                MessageBox(NULL, (LPCTSTR) "Cliente aceito!", (LPCTSTR) "Campo de batalha", MB_OK);
                quit = 1;
            }
            while (quit == 1) {
                if (SDL_PollEvent(&event))  // menu do jogo.
                {
                    if (event.type == SDL_KEYDOWN)  // evento do tipo tecla pressionada
                    {
                        switch (event.key.keysym.sym) {
                            case SDLK_1:
                                bytesreceive = recv(sClient, buffer, 200, 0);
                                bytessend = send(sClient, buffer, 200, 0);
                                MessageBox(NULL, (LPCTSTR) "dados enviados com sucesso!",
                                           (LPCTSTR) "Campo de batalha", MB_OK);
                                break;
                            case SDLK_2:
                                quit = 0;
                                break;
                            default:
                                x = 1;
                        }
                    }
                    if (event.type == SDL_QUIT)  // fechar o programa caso cliquem no X
                    {
                        quit1 = 1;
                        quit = 0;
                    }
                }
            }
            entrar = 1;
            // Cleanup
            closesocket(sClient);
            closesocket(sServer);
            // Shutdown Winsock
            WSACleanup();
        }
        if (entrar == 6)  // condi��o de entrada no cliente
        {
            SDL_FreeSurface(telaprincipal);
            telaprincipal =
                SDL_LoadBMP("resources/mododeespera.bmp");  // troca a imagem bmp para o menu
            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
            SDL_Flip(screen);

            WSADATA t_wsa;  // WSADATA structure
            WORD wVers;     // version number
            int iError;     // error number

            wVers = MAKEWORD(2, 2);              // Set the version number to 2.2
            iError = WSAStartup(wVers, &t_wsa);  // Start the WSADATA

            SOCKET sClient;
            sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            SOCKADDR_IN sinClient;
            memset(&sinClient, 0, sizeof(sinClient));

            char cIP[50];
            strcpy(cIP, "127.0.0.1");
            MessageBox(NULL, (LPCTSTR) "digite o ip para se conectar!",
                       (LPCTSTR) "Campo de batalha", MB_OK);
            scanf("%s", cIP);
            sinClient.sin_family = AF_INET;
            sinClient.sin_addr.s_addr = inet_addr(cIP);  // Where to start server?
            sinClient.sin_port = htons(1000);            // Port

            if (connect(sClient, (LPSOCKADDR)&sinClient, sizeof(sinClient)) == SOCKET_ERROR) {
                /* failed at starting server */
                MessageBox(NULL, (LPCTSTR) "nao foi possivel conectar-se a um servidor!",
                           (LPCTSTR) "Campo de batalha", MB_OK);
                WSACleanup();
                entrar = 4;
            }
            // Now we can send/recv data!
            int bytessend, bytesreceive, quit, x, y;
            char cBuffer[200];
            char buffer[200];
            quit = 0;

            MessageBox(NULL, (LPCTSTR) "voce esta conectado ao servidor!",
                       (LPCTSTR) "Campo de batalha", MB_OK);

            entrar = 4;
        }
        if (entrar == 2)  // condicao 2 (o jogo em si)
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

            offset.x = 15, offset.y = 170, offset.w = 32, offset.h = 32;
            offset2.x = 560, offset2.y = 260, offset2.w = 32, offset2.h = 32;

            wall.x = 287, wall.y = 102, wall.w = 53, wall.h = 240;
            menu.x = 1, menu.y = 413, menu.w = 638, menu.h = 66;

            bala01.x = 0, bala01.y = 0, bala01.w = 10, bala01.h = 10;

            pontuacao01[0].x = 0, pontuacao01[0].y = 3, pontuacao01[0].w = 25,
            pontuacao01[0].h = 35;
            pontuacao01[1].x = 50, pontuacao01[1].y = 3, pontuacao01[1].w = 25,
            pontuacao01[1].h = 35;
            pontuacao01[2].x = 98, pontuacao01[2].y = 3, pontuacao01[2].w = 25,
            pontuacao01[2].h = 35;
            pontuacao01[3].x = 147, pontuacao01[3].y = 3, pontuacao01[3].w = 25,
            pontuacao01[3].h = 35;
            pontuacao01[4].x = 195, pontuacao01[4].y = 3, pontuacao01[4].w = 25,
            pontuacao01[4].h = 35;
            pontuacao01[5].x = 245, pontuacao01[5].y = 3, pontuacao01[5].w = 25,
            pontuacao01[5].h = 35;
            pontuacao01[6].x = 294, pontuacao01[6].y = 3, pontuacao01[6].w = 25,
            pontuacao01[6].h = 35;

            jogador1_c[0].x = 0, jogador1_c[0].y = 0, jogador1_c[0].w = 32,
            jogador1_c[0].h = 32;  // jogador1 virado para direita
            jogador1_c[1].x = 34, jogador1_c[1].y = 34, jogador1_c[1].w = 32,
            jogador1_c[1].h = 32;  // jogador1 virado para cima
            jogador1_c[2].x = 34, jogador1_c[2].y = 0, jogador1_c[2].w = 32,
            jogador1_c[2].h = 32;  // jogador1 virado para baixo
            jogador1_c[3].x = 0, jogador1_c[3].y = 34, jogador1_c[3].w = 32,
            jogador1_c[3].h = 32;  // jogador1 virado para esquerda

            jogador2_c[0].x = 0, jogador2_c[0].y = 0, jogador2_c[0].w = 32,
            jogador2_c[0].h = 32;  // jogador2 virado para direita
            jogador2_c[1].x = 34, jogador2_c[1].y = 34, jogador2_c[1].w = 32,
            jogador2_c[1].h = 32;  // jogador2 virado para cima
            jogador2_c[2].x = 34, jogador2_c[2].y = 0, jogador2_c[2].w = 32,
            jogador2_c[2].h = 32;  // jogador2 virado para baixo
            jogador2_c[3].x = 0, jogador2_c[3].y = 34, jogador2_c[3].w = 32,
            jogador2_c[3].h = 32;  // jogador2 virado para esquerda

            SDL_BlitSurface(pontuacao, &pontuacao01[0], screen, &pontuacao02);
            SDL_BlitSurface(pontuacao, &pontuacao01[0], screen, &pontuacao03);
            SDL_BlitSurface(jogador1, &jogador1_c[0], screen, &offset);
            SDL_BlitSurface(jogador2, &jogador2_c[3], screen, &offset2);
            SDL_Flip(screen);
            posicao1 = 2;
            posicao2 = 3;
            // fim da atribui��o de variaveis

            while (quit2 == 0)  // loop para controlar o fim do jogo
            {
                while (SDL_PollEvent(&event))  // loop de eventos
                {
                    switch (event.type) {
                        case SDL_KEYDOWN:
                            switch (event.key.keysym.sym) {  // comandos para o jogador 1
                                case SDLK_UP:                // tecla para cima
                                    if (colisao(offset.x, offset.y, offset.w, offset.h, wall.x,
                                                wall.y, wall.w, wall.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x,
                                                offset2.y, offset2.w, offset2.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, menu.x,
                                                menu.y, menu.w, menu.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset3.x,
                                                offset3.y, offset3.w, offset3.h)) {
                                        offset.y = offset.y - 6;
                                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                        SDL_BlitSurface(jogador1, &jogador1_c[1], screen, &offset);
                                        jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                     screen);
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_Flip(screen);
                                        posicao1 = 0;
                                        break;
                                    } else {
                                        pontuacao2 = pontuacao2 + 1;
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_BlitSurface(explosao, NULL, screen, &offset);
                                        SDL_Flip(screen);
                                        SDL_Delay(500);
                                        offset.x = 15, offset.y = 170;
                                    }
                                    break;
                                case SDLK_DOWN:  // tecla para baixo
                                    if (colisao(offset.x, offset.y, offset.w, offset.h, wall.x,
                                                wall.y, wall.w, wall.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x,
                                                offset2.y, offset2.w, offset2.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, menu.x,
                                                menu.y, menu.w, menu.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset3.x,
                                                offset3.y, offset3.w, offset3.h)) {
                                        offset.y = offset.y + 6;
                                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                        SDL_BlitSurface(jogador1, &jogador1_c[2], screen, &offset);
                                        jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                     screen);
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_Flip(screen);
                                        posicao1 = 1;
                                        break;
                                    } else {
                                        pontuacao2 = pontuacao2 + 1;
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_BlitSurface(explosao, NULL, screen, &offset);
                                        SDL_Flip(screen);
                                        SDL_Delay(500);
                                        offset.x = 15, offset.y = 170;
                                    }
                                    break;
                                case SDLK_RIGHT:  // tecla para a direita
                                    if (colisao(offset.x, offset.y, offset.w, offset.h, wall.x,
                                                wall.y, wall.w, wall.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x,
                                                offset2.y, offset2.w, offset2.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, menu.x,
                                                menu.y, menu.w, menu.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset3.x,
                                                offset3.y, offset3.w, offset3.h)) {
                                        offset.x = offset.x + 6;
                                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                        SDL_BlitSurface(jogador1, &jogador1_c[0], screen, &offset);
                                        jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                     screen);
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_Flip(screen);
                                        posicao1 = 2;
                                        break;
                                    } else {
                                        pontuacao2 = pontuacao2 + 1;
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_BlitSurface(explosao, NULL, screen, &offset);
                                        SDL_Flip(screen);
                                        SDL_Delay(500);
                                        offset.x = 15, offset.y = 170;
                                    }
                                    break;
                                case SDLK_LEFT:  // tecla para esquerda
                                    if (colisao(offset.x, offset.y, offset.w, offset.h, wall.x,
                                                wall.y, wall.w, wall.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x,
                                                offset2.y, offset2.w, offset2.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, menu.x,
                                                menu.y, menu.w, menu.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset3.x,
                                                offset3.y, offset3.w, offset3.h)) {
                                        offset.x = offset.x - 6;
                                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                        SDL_BlitSurface(jogador1, &jogador1_c[3], screen, &offset);
                                        jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                     screen);
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_Flip(screen);
                                        posicao1 = 3;
                                        break;
                                    } else {
                                        pontuacao2 = pontuacao2 + 1;
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_BlitSurface(explosao, NULL, screen, &offset);
                                        SDL_Flip(screen);
                                        SDL_Delay(500);
                                        offset.x = 15, offset.y = 170;
                                    }
                                    break;
                                // comandos para o jogador 2
                                case SDLK_w:  // tecla w ( para cima )
                                    if (colisao(offset2.x, offset2.y, offset2.w, offset2.h, wall.x,
                                                wall.y, wall.w, wall.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x,
                                                offset2.y, offset2.w, offset2.h) &&
                                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, menu.x,
                                                menu.y, menu.w, menu.h) &&
                                        colisao(offset2.x, offset2.y, offset2.w, offset2.h,
                                                offset3.x, offset3.y, offset3.w, offset3.h)) {
                                        offset2.y = offset2.y - 6;
                                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                        SDL_BlitSurface(jogador2, &jogador2_c[1], screen, &offset2);
                                        jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                     screen);
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_Flip(screen);
                                        posicao2 = 0;
                                        break;
                                    } else {
                                        pontuacao1 = pontuacao1 + 1;
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_BlitSurface(explosao, NULL, screen, &offset2);
                                        SDL_Flip(screen);
                                        SDL_Delay(500);
                                        offset2.x = 560, offset2.y = 260;
                                    }
                                    break;
                                case SDLK_s:  // tecla s ( para baixo )
                                    if (colisao(offset2.x, offset2.y, offset2.w, offset2.h, wall.x,
                                                wall.y, wall.w, wall.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x,
                                                offset2.y, offset2.w, offset2.h) &&
                                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, menu.x,
                                                menu.y, menu.w, menu.h) &&
                                        colisao(offset2.x, offset2.y, offset2.w, offset2.h,
                                                offset3.x, offset3.y, offset3.w, offset3.h)) {
                                        offset2.y = offset2.y + 6;
                                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                        SDL_BlitSurface(jogador2, &jogador2_c[2], screen, &offset2);
                                        jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                     screen);
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_Flip(screen);
                                        posicao2 = 1;
                                        break;
                                    } else {
                                        pontuacao1 = pontuacao1 + 1;
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_BlitSurface(explosao, NULL, screen, &offset2);
                                        SDL_Flip(screen);
                                        SDL_Delay(500);
                                        offset2.x = 560, offset2.y = 260;
                                    }
                                    break;
                                case SDLK_d:  // tecla d ( para direita )
                                    if (colisao(offset2.x, offset2.y, offset2.w, offset2.h, wall.x,
                                                wall.y, wall.w, wall.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x,
                                                offset2.y, offset2.w, offset2.h) &&
                                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, menu.x,
                                                menu.y, menu.w, menu.h) &&
                                        colisao(offset2.x, offset2.y, offset2.w, offset2.h,
                                                offset3.x, offset3.y, offset3.w, offset3.h)) {
                                        offset2.x = offset2.x + 6;
                                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                        SDL_BlitSurface(jogador2, &jogador2_c[0], screen, &offset2);
                                        jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                     screen);
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_Flip(screen);
                                        posicao2 = 2;
                                        break;
                                    } else {
                                        pontuacao1 = pontuacao1 + 1;
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_BlitSurface(explosao, NULL, screen, &offset2);
                                        SDL_Flip(screen);
                                        SDL_Delay(500);
                                        offset2.x = 560, offset2.y = 260;
                                    }
                                    break;
                                case SDLK_a:  // tecla a ( para esquerda )
                                    if (colisao(offset2.x, offset2.y, offset2.w, offset2.h, wall.x,
                                                wall.y, wall.w, wall.h) &&
                                        colisao(offset.x, offset.y, offset.w, offset.h, offset2.x,
                                                offset2.y, offset2.w, offset2.h) &&
                                        colisao(offset2.x, offset2.y, offset2.w, offset2.h, menu.x,
                                                menu.y, menu.w, menu.h) &&
                                        colisao(offset2.x, offset2.y, offset2.w, offset2.h,
                                                offset3.x, offset3.y, offset3.w, offset3.h)) {
                                        offset2.x = offset2.x - 6;
                                        SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                        SDL_BlitSurface(jogador2, &jogador2_c[3], screen, &offset2);
                                        jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                     screen);
                                        pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                       pontuacao01, screen);
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_Flip(screen);
                                        posicao2 = 3;
                                        break;
                                    } else {
                                        pontuacao1 = pontuacao1 + 1;
                                        pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                       pontuacao01, screen);
                                        SDL_BlitSurface(explosao, NULL, screen, &offset2);
                                        SDL_Flip(screen);
                                        SDL_Delay(500);
                                        offset2.x = 560, offset2.y = 260;
                                    }
                                    break;
                                case SDLK_ESCAPE:
                                    quit2 = 1;
                                    break;
                                // comandos para o tiro dos jogadores 1 e 2
                                case SDLK_m:  // tecla m para o tiro do jogador 1
                                    switch (posicao1) {
                                        case 0:
                                            offset3.x = offset.x + 12;
                                            offset3.y = offset.y - 12;
                                            for (bala1 = 1; bala1 < 10; bala1 = bala1 + 1) {
                                                offset3.y = offset3.y - 10;
                                                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                                jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                             screen);
                                                pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                               pontuacao01, screen);
                                                pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                               pontuacao01, screen);
                                                jogador_load(posicao2, offset2, jogador2,
                                                             jogador2_c, screen);
                                                SDL_BlitSurface(bala, NULL, screen, &offset3);
                                                SDL_Flip(screen);
                                                SDL_Delay(50);
                                                andar(event, screen, telaprincipal, jogador1,
                                                      jogador2, bala, explosao, pontuacao,
                                                      pontuacao01, jogador1_c, jogador2_c, wall,
                                                      menu, offset3, pontuacao02, pontuacao03,
                                                      bala01, vencedor, quit1, quit2, x, entrar,
                                                      bala1, bala2, pontuacao1, pontuacao2,
                                                      vencedor0);
                                                if (!colisao(offset2.x, offset2.y, offset2.w,
                                                             offset2.h, offset3.x, offset3.y,
                                                             offset3.w, offset3.h)) {
                                                    pontuacao1 = pontuacao1 + 1;
                                                    pontuacao_load(pontuacao1, pontuacao02,
                                                                   pontuacao, pontuacao01, screen);
                                                    SDL_BlitSurface(explosao, NULL, screen,
                                                                    &offset2);
                                                    SDL_Flip(screen);
                                                    SDL_Delay(100);
                                                    offset2.x = 560, offset2.y = 260;
                                                    break;
                                                }
                                            }
                                            offset3.x = 650;
                                            offset3.y = 650;
                                            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                            jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                         screen);
                                            pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                           pontuacao01, screen);
                                            pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                           pontuacao01, screen);
                                            jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                         screen);
                                            SDL_Flip(screen);
                                            break;
                                        case 1:
                                            offset3.x = offset.x + 12;
                                            offset3.y = offset.y + 36;
                                            for (bala1 = 1; bala1 < 10; bala1 = bala1 + 1) {
                                                offset3.y = offset3.y + 10;
                                                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                                jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                             screen);
                                                pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                               pontuacao01, screen);
                                                pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                               pontuacao01, screen);
                                                jogador_load(posicao2, offset2, jogador2,
                                                             jogador2_c, screen);
                                                SDL_BlitSurface(bala, NULL, screen, &offset3);
                                                SDL_Flip(screen);
                                                SDL_Delay(50);
                                                andar(event, screen, telaprincipal, jogador1,
                                                      jogador2, bala, explosao, pontuacao,
                                                      pontuacao01, jogador1_c, jogador2_c, wall,
                                                      menu, offset3, pontuacao02, pontuacao03,
                                                      bala01, vencedor, quit1, quit2, x, entrar,
                                                      bala1, bala2, pontuacao1, pontuacao2,
                                                      vencedor0);
                                                if (!colisao(offset2.x, offset2.y, offset2.w,
                                                             offset2.h, offset3.x, offset3.y,
                                                             offset3.w, offset3.h)) {
                                                    pontuacao1 = pontuacao1 + 1;
                                                    pontuacao_load(pontuacao1, pontuacao02,
                                                                   pontuacao, pontuacao01, screen);
                                                    SDL_BlitSurface(explosao, NULL, screen,
                                                                    &offset2);
                                                    SDL_Flip(screen);
                                                    SDL_Delay(100);
                                                    offset2.x = 560, offset2.y = 260;
                                                    break;
                                                }
                                            }
                                            offset3.x = 650;
                                            offset3.y = 650;
                                            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                            jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                         screen);
                                            pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                           pontuacao01, screen);
                                            pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                           pontuacao01, screen);
                                            jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                         screen);
                                            SDL_Flip(screen);
                                            break;
                                        case 2:
                                            offset3.x = offset.x + 36;
                                            offset3.y = offset.y + 12;
                                            for (bala1 = 1; bala1 < 10; bala1 = bala1 + 1) {
                                                offset3.x = offset3.x + 10;
                                                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                                jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                             screen);
                                                pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                               pontuacao01, screen);
                                                pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                               pontuacao01, screen);
                                                jogador_load(posicao2, offset2, jogador2,
                                                             jogador2_c, screen);
                                                SDL_BlitSurface(bala, NULL, screen, &offset3);
                                                SDL_Flip(screen);
                                                SDL_Delay(50);
                                                andar(event, screen, telaprincipal, jogador1,
                                                      jogador2, bala, explosao, pontuacao,
                                                      pontuacao01, jogador1_c, jogador2_c, wall,
                                                      menu, offset3, pontuacao02, pontuacao03,
                                                      bala01, vencedor, quit1, quit2, x, entrar,
                                                      bala1, bala2, pontuacao1, pontuacao2,
                                                      vencedor0);
                                                if (!colisao(offset2.x, offset2.y, offset2.w,
                                                             offset2.h, offset3.x, offset3.y,
                                                             offset3.w, offset3.h)) {
                                                    pontuacao1 = pontuacao1 + 1;
                                                    pontuacao_load(pontuacao1, pontuacao02,
                                                                   pontuacao, pontuacao01, screen);
                                                    SDL_BlitSurface(explosao, NULL, screen,
                                                                    &offset2);
                                                    SDL_Flip(screen);
                                                    SDL_Delay(100);
                                                    offset2.x = 560, offset2.y = 260;
                                                    break;
                                                }
                                            }
                                            offset3.x = 650;
                                            offset3.y = 650;
                                            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                            jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                         screen);
                                            pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                           pontuacao01, screen);
                                            pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                           pontuacao01, screen);
                                            jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                         screen);
                                            SDL_Flip(screen);
                                            break;
                                        case 3:
                                            offset3.x = offset.x - 12;
                                            offset3.y = offset.y + 12;
                                            for (bala1 = 1; bala1 < 10; bala1 = bala1 + 1) {
                                                offset3.x = offset3.x - 10;
                                                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                                jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                             screen);
                                                pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                               pontuacao01, screen);
                                                pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                               pontuacao01, screen);
                                                jogador_load(posicao2, offset2, jogador2,
                                                             jogador2_c, screen);
                                                SDL_BlitSurface(bala, NULL, screen, &offset3);
                                                SDL_Flip(screen);
                                                SDL_Delay(50);
                                                andar(event, screen, telaprincipal, jogador1,
                                                      jogador2, bala, explosao, pontuacao,
                                                      pontuacao01, jogador1_c, jogador2_c, wall,
                                                      menu, offset3, pontuacao02, pontuacao03,
                                                      bala01, vencedor, quit1, quit2, x, entrar,
                                                      bala1, bala2, pontuacao1, pontuacao2,
                                                      vencedor0);
                                                if (!colisao(offset2.x, offset2.y, offset2.w,
                                                             offset2.h, offset3.x, offset3.y,
                                                             offset3.w, offset3.h)) {
                                                    pontuacao1 = pontuacao1 + 1;
                                                    pontuacao_load(pontuacao1, pontuacao02,
                                                                   pontuacao, pontuacao01, screen);
                                                    SDL_BlitSurface(explosao, NULL, screen,
                                                                    &offset2);
                                                    SDL_Flip(screen);
                                                    SDL_Delay(100);
                                                    offset2.x = 560, offset2.y = 260;
                                                    break;
                                                }
                                            }
                                            offset3.x = 650;
                                            offset3.y = 650;
                                            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                            jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                         screen);
                                            pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                           pontuacao01, screen);
                                            pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                           pontuacao01, screen);
                                            jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                         screen);
                                            SDL_Flip(screen);
                                            break;
                                    }
                                    break;
                                case SDLK_f:  // tecla f para o jogador 2
                                    switch (posicao2) {
                                        case 0:
                                            offset3.x = offset2.x + 12;
                                            offset3.y = offset2.y - 12;
                                            for (bala1 = 1; bala1 < 10; bala1 = bala1 + 1) {
                                                offset3.y = offset3.y - 10;
                                                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                                jogador_load(posicao2, offset2, jogador2,
                                                             jogador2_c, screen);
                                                pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                               pontuacao01, screen);
                                                pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                               pontuacao01, screen);
                                                jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                             screen);
                                                SDL_BlitSurface(bala, NULL, screen, &offset3);
                                                SDL_Flip(screen);
                                                SDL_Delay(50);
                                                andar(event, screen, telaprincipal, jogador1,
                                                      jogador2, bala, explosao, pontuacao,
                                                      pontuacao01, jogador1_c, jogador2_c, wall,
                                                      menu, offset3, pontuacao02, pontuacao03,
                                                      bala01, vencedor, quit1, quit2, x, entrar,
                                                      bala1, bala2, pontuacao1, pontuacao2,
                                                      vencedor0);
                                                if (!colisao(offset.x, offset.y, offset.w, offset.h,
                                                             offset3.x, offset3.y, offset3.w,
                                                             offset3.h)) {
                                                    pontuacao2 = pontuacao2 + 1;
                                                    pontuacao_load(pontuacao2, pontuacao03,
                                                                   pontuacao, pontuacao01, screen);
                                                    SDL_BlitSurface(explosao, NULL, screen,
                                                                    &offset);
                                                    SDL_Flip(screen);
                                                    SDL_Delay(100);
                                                    offset.x = 15, offset.y = 170;
                                                    break;
                                                }
                                            }
                                            offset3.x = 650;
                                            offset3.y = 650;
                                            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                            jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                         screen);
                                            pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                           pontuacao01, screen);
                                            pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                           pontuacao01, screen);
                                            jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                         screen);
                                            SDL_Flip(screen);
                                            break;
                                        case 1:
                                            offset3.x = offset2.x + 12;
                                            offset3.y = offset2.y + 36;
                                            for (bala1 = 1; bala1 < 10; bala1 = bala1 + 1) {
                                                offset3.y = offset3.y + 10;
                                                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                                jogador_load(posicao2, offset2, jogador2,
                                                             jogador2_c, screen);
                                                pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                               pontuacao01, screen);
                                                pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                               pontuacao01, screen);
                                                jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                             screen);
                                                SDL_BlitSurface(bala, NULL, screen, &offset3);
                                                SDL_Flip(screen);
                                                SDL_Delay(50);
                                                andar(event, screen, telaprincipal, jogador1,
                                                      jogador2, bala, explosao, pontuacao,
                                                      pontuacao01, jogador1_c, jogador2_c, wall,
                                                      menu, offset3, pontuacao02, pontuacao03,
                                                      bala01, vencedor, quit1, quit2, x, entrar,
                                                      bala1, bala2, pontuacao1, pontuacao2,
                                                      vencedor0);
                                                if (!colisao(offset.x, offset.y, offset.w, offset.h,
                                                             offset3.x, offset3.y, offset3.w,
                                                             offset3.h)) {
                                                    pontuacao2 = pontuacao2 + 1;
                                                    pontuacao_load(pontuacao2, pontuacao03,
                                                                   pontuacao, pontuacao01, screen);
                                                    SDL_BlitSurface(explosao, NULL, screen,
                                                                    &offset);
                                                    SDL_Flip(screen);
                                                    SDL_Delay(100);
                                                    offset.x = 15, offset.y = 170;
                                                    break;
                                                }
                                            }
                                            offset3.x = 650;
                                            offset3.y = 650;
                                            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                            jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                         screen);
                                            pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                           pontuacao01, screen);
                                            pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                           pontuacao01, screen);
                                            jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                         screen);
                                            SDL_Flip(screen);
                                            break;
                                        case 2:
                                            offset3.x = offset2.x + 36;
                                            offset3.y = offset2.y + 12;
                                            for (bala1 = 1; bala1 < 10; bala1 = bala1 + 1) {
                                                offset3.x = offset3.x + 10;
                                                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                                jogador_load(posicao2, offset2, jogador2,
                                                             jogador2_c, screen);
                                                pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                               pontuacao01, screen);
                                                pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                               pontuacao01, screen);
                                                jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                             screen);
                                                SDL_BlitSurface(bala, NULL, screen, &offset3);
                                                SDL_Flip(screen);
                                                SDL_Delay(50);
                                                andar(event, screen, telaprincipal, jogador1,
                                                      jogador2, bala, explosao, pontuacao,
                                                      pontuacao01, jogador1_c, jogador2_c, wall,
                                                      menu, offset3, pontuacao02, pontuacao03,
                                                      bala01, vencedor, quit1, quit2, x, entrar,
                                                      bala1, bala2, pontuacao1, pontuacao2,
                                                      vencedor0);
                                                if (!colisao(offset.x, offset.y, offset.w, offset.h,
                                                             offset3.x, offset3.y, offset3.w,
                                                             offset3.h)) {
                                                    pontuacao2 = pontuacao2 + 1;
                                                    pontuacao_load(pontuacao2, pontuacao03,
                                                                   pontuacao, pontuacao01, screen);
                                                    SDL_BlitSurface(explosao, NULL, screen,
                                                                    &offset);
                                                    SDL_Flip(screen);
                                                    SDL_Delay(100);
                                                    offset.x = 15, offset.y = 170;
                                                    break;
                                                }
                                            }
                                            offset3.x = 650;
                                            offset3.y = 650;
                                            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                            jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                         screen);
                                            pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                           pontuacao01, screen);
                                            pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                           pontuacao01, screen);
                                            jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                         screen);
                                            SDL_Flip(screen);
                                            break;
                                        case 3:
                                            offset3.x = offset2.x - 12;
                                            offset3.y = offset2.y + 12;
                                            for (bala1 = 1; bala1 < 10; bala1 = bala1 + 1) {
                                                offset3.x = offset3.x - 10;
                                                SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                                jogador_load(posicao2, offset2, jogador2,
                                                             jogador2_c, screen);
                                                pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                               pontuacao01, screen);
                                                pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                               pontuacao01, screen);
                                                jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                             screen);
                                                SDL_BlitSurface(bala, NULL, screen, &offset3);
                                                SDL_Flip(screen);
                                                SDL_Delay(50);
                                                andar(event, screen, telaprincipal, jogador1,
                                                      jogador2, bala, explosao, pontuacao,
                                                      pontuacao01, jogador1_c, jogador2_c, wall,
                                                      menu, offset3, pontuacao02, pontuacao03,
                                                      bala01, vencedor, quit1, quit2, x, entrar,
                                                      bala1, bala2, pontuacao1, pontuacao2,
                                                      vencedor0);
                                                if (!colisao(offset.x, offset.y, offset.w, offset.h,
                                                             offset3.x, offset3.y, offset3.w,
                                                             offset3.h)) {
                                                    pontuacao2 = pontuacao2 + 1;
                                                    pontuacao_load(pontuacao2, pontuacao03,
                                                                   pontuacao, pontuacao01, screen);
                                                    SDL_BlitSurface(explosao, NULL, screen,
                                                                    &offset);
                                                    SDL_Flip(screen);
                                                    SDL_Delay(100);
                                                    offset.x = 15, offset.y = 170;
                                                    break;
                                                }
                                            }
                                            offset3.x = 650;
                                            offset3.y = 650;
                                            SDL_BlitSurface(telaprincipal, NULL, screen, NULL);
                                            jogador_load(posicao2, offset2, jogador2, jogador2_c,
                                                         screen);
                                            pontuacao_load(pontuacao2, pontuacao03, pontuacao,
                                                           pontuacao01, screen);
                                            pontuacao_load(pontuacao1, pontuacao02, pontuacao,
                                                           pontuacao01, screen);
                                            jogador_load(posicao1, offset, jogador1, jogador1_c,
                                                         screen);
                                            SDL_Flip(screen);
                                            break;
                                        default:
                                            x = 0;
                                    }
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
        }
    }
    SDL_Quit();
    return 0;
}
