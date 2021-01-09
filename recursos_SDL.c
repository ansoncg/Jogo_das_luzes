#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "recursos_SDL.h"

#define FPS 60

enum {X, Y};

struct sdl_vars {
    SDL_Window *janela;
    SDL_Renderer *renderizador;
    SDL_Texture *grid, *luz;
    SDL_Event evento;
    TTF_Font *fonte;
    unsigned int TEMPO_DELAY, frameIni, frameTempo;
    int mousePos[2];
    bool mouseClicou, verResposta;
};

// Auxiliares

SDL_Texture* pegaTextura(char *nomeArquivo, SDL_Renderer *renderizador) { 
    SDL_Texture* textura = NULL;
    SDL_Surface* superficieTemp = IMG_Load(nomeArquivo);

    if (!superficieTemp)
        return NULL;
    textura = SDL_CreateTextureFromSurface(renderizador, superficieTemp);
    SDL_FreeSurface(superficieTemp);
    if (textura != NULL) 
        return textura;
    else
        return NULL;
}

void atualizaMouse(int mousePos[2], SDL_Event evento) {
    mousePos[X] = evento.motion.x;
    mousePos[Y] = evento.motion.y;
    return;
}

void desenhaNaTela(int x, int y, int largura, int altura, SDL_Texture *textura, SDL_Renderer *renderizador) { 
    SDL_Rect fontRect, destRect;

    fontRect.x = fontRect.y = 0;
    fontRect.w = destRect.w = largura; 
    fontRect.h = destRect.h = altura; 
    destRect.x = x;
    destRect.y = y;
    SDL_RenderCopy(renderizador, textura, &fontRect, &destRect);
    return;
}

void escreveNaTela(char *texto, TTF_Font *fonte, SDL_Renderer *renderizador, int x, int y) {
    int altura, largura;

    SDL_Color cor = {0, 0, 0, 255}; // Cor da fonte.
    SDL_Surface *superficieMsg = TTF_RenderText_Solid(fonte, texto, cor);
    SDL_Texture *mensagem = SDL_CreateTextureFromSurface(renderizador, superficieMsg);  
    SDL_FreeSurface(superficieMsg);
    SDL_QueryTexture(mensagem, NULL, NULL, &largura, &altura);
    desenhaNaTela(x, y, largura, altura, mensagem, renderizador); 
    return;
}

bool pegaInput(SDL_Event evento, int mousePos[2], bool *mouseClicou) {
    while (SDL_PollEvent(&evento)) {
        switch (evento.type) {
            case SDL_QUIT:
                return false;
                break;
            case SDL_MOUSEMOTION:
                atualizaMouse(mousePos, evento);
                break;
            case SDL_MOUSEBUTTONDOWN:
                *mouseClicou = true;
                //printf("%d %d\n", (mousePos[X]/100) - 1, (mousePos[Y]/100) - 1);
                break;
        }
    }
    return true;
}

void desenhaJogo(SDL_Vars *sdl_vars, int gridLuz[5][5], char *solucao) {
    bool vitoria = true;

    SDL_RenderClear(sdl_vars->renderizador);
    desenhaNaTela(100, 100, 500, 500, sdl_vars->grid, sdl_vars->renderizador);
    for (int i = 0; i < 5; i++)  
        for (int j = 0; j < 5; j++)
            if (gridLuz[i][j]) {
                desenhaNaTela(i*100 + 100, j*100 + 100, 100, 100, sdl_vars->luz, sdl_vars->renderizador);
                vitoria = false;
            }
    escreveNaTela("Reiniciar", sdl_vars->fonte, sdl_vars->renderizador, 675, 500);
    if (vitoria)
        escreveNaTela("Ganhou!", sdl_vars->fonte, sdl_vars->renderizador, 275, 25);
    escreveNaTela("Ver resposta", sdl_vars->fonte, sdl_vars->renderizador, 635, 200);
    if (sdl_vars->verResposta)
        escreveNaTela(solucao, sdl_vars->fonte, sdl_vars->renderizador, 675, 300);
    SDL_RenderPresent(sdl_vars->renderizador);
    return;
}

// Funcionalidades

bool inicializaSDL(SDL_Vars *sdl_vars) { 
    if (SDL_Init(SDL_INIT_EVERYTHING))
        return false;
    if(!(sdl_vars->janela = SDL_CreateWindow("Jogo das luzes", 0, 0, 1000, 700, false))) 
        return false;
    if(!(sdl_vars->renderizador = SDL_CreateRenderer(sdl_vars->janela, -1, 0))) 
        return false;
    SDL_SetRenderDrawColor(sdl_vars->renderizador, 255, 255, 255, 255); 
    TTF_Init();
    sdl_vars->TEMPO_DELAY = (1000 / FPS);
    sdl_vars->grid = pegaTextura("recursos/grid.png", sdl_vars->renderizador);
    sdl_vars->luz = pegaTextura("recursos/luz.png", sdl_vars->renderizador);
    sdl_vars->fonte = TTF_OpenFont("recursos/consolas.ttf", 50);
    return true;
}

void finalizaSDL(SDL_Vars *sdl_vars) {
    SDL_DestroyTexture(sdl_vars->grid);
    SDL_DestroyTexture(sdl_vars->luz);
    TTF_CloseFont(sdl_vars->fonte);
    SDL_DestroyWindow(sdl_vars->janela);
    SDL_DestroyRenderer(sdl_vars->renderizador);
    SDL_Quit();
    return;
}

void criaSDL_Vars(SDL_Vars **sdl_vars) {
    *sdl_vars = calloc(1, sizeof(SDL_Vars));
    (*sdl_vars)->janela = NULL;
    (*sdl_vars)->renderizador = NULL;
    (*sdl_vars)->grid = NULL;
    (*sdl_vars)->luz = NULL;
    (*sdl_vars)->fonte = NULL;
    (*sdl_vars)->mouseClicou = false;
    (*sdl_vars)->verResposta = false;
    return;
}

void destroiSDL_Vars(SDL_Vars **sdl_vars) {
    free(*sdl_vars);
    *sdl_vars = NULL;
    return;
}

void marcaInicioLoop(SDL_Vars *sdl_vars) {
    sdl_vars->frameIni = SDL_GetTicks();
    return;
}

void marcaFimLoop(SDL_Vars *sdl_vars) {
    sdl_vars->frameTempo = SDL_GetTicks() - sdl_vars->frameIni;
    if (sdl_vars->frameTempo < sdl_vars->TEMPO_DELAY)
        SDL_Delay(sdl_vars->TEMPO_DELAY - sdl_vars->frameTempo);
    return;
}

void reinicia(SDL_Vars *sdl_vars, Jogo *jogo) {
    novoJogo(jogo);
    sdl_vars->verResposta = false;
    return;
}

void atualizaTabuleiro(SDL_Vars *sdl_vars, Jogo *jogo) {
    if (sdl_vars->mouseClicou) {
        movimentaTabuleiro(jogo, (sdl_vars->mousePos[X] / 100) - 1, (sdl_vars->mousePos[Y] / 100) - 1);
        if (sdl_vars->mousePos[X] > 635 && sdl_vars->mousePos[X] < 960 && sdl_vars->mousePos[Y] > 200 && sdl_vars->mousePos[Y] < 250) 
            sdl_vars->verResposta = true;
        if (sdl_vars->mousePos[X] > 675 && sdl_vars->mousePos[X] < 920 && sdl_vars->mousePos[Y] > 500 && sdl_vars->mousePos[Y] < 550) 
            reinicia(sdl_vars, jogo);
        sdl_vars->mouseClicou = false;
    }
    return;
}

bool atualiza(SDL_Vars *sdl_vars, Jogo *jogo) {
    int grid[5][5];
    char solucao[100];

    marcaInicioLoop(sdl_vars);
    if (!pegaInput(sdl_vars->evento, sdl_vars->mousePos, &sdl_vars->mouseClicou)) {
        #ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
        #endif
        return false;
    }
    atualizaTabuleiro(sdl_vars, jogo);
    pegaGrid(jogo, grid); 
    pegaSolucao(jogo, solucao);
    desenhaJogo(sdl_vars, grid, solucao);
    marcaFimLoop(sdl_vars);
    return true;
}
