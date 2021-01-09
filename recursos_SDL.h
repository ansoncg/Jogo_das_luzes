#ifndef VISUAL_H
#define VISUAL_H

#include <stdbool.h>
#include "logica.h"

typedef struct sdl_vars SDL_Vars;

void criaSDL_Vars(SDL_Vars **sdl_vars); 
void destroiSDL_Vars(SDL_Vars **sdl_vars); 
bool inicializaSDL(SDL_Vars *sdl_vars);
void finalizaSDL(SDL_Vars *sdl_vars); 
void marcaInicioLoop(SDL_Vars *sdl_vars); 
void marcaFimLoop(SDL_Vars *sdl_vars); 
bool atualiza(SDL_Vars *sdl_vars, Jogo *jogo); 

#endif
