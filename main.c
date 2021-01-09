#include <stddef.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "recursos_SDL.h"
#include "logica.h"

Jogo *jogo = NULL;
SDL_Vars *sdl_vars = NULL;

void main_loop(void) {
    atualiza(sdl_vars, jogo);
    return;
}

int main() {
    bool rodando;

    criaJogo(&jogo);
    criaSDL_Vars(&sdl_vars);
    rodando = inicializaSDL(sdl_vars);
    novoJogo(jogo);

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
    #else
        while (rodando) 
            rodando = atualiza(sdl_vars, jogo);
    #endif

    finalizaSDL(sdl_vars); 
    destroiSDL_Vars(&sdl_vars);
    destroiJogo(&jogo);
    return 0;
}
