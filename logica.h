#ifndef LUZES_H
#define LUZES_H

#include <stdbool.h>

typedef struct jogo Jogo;

void criaJogo(Jogo **jogo); 
void destroiJogo(Jogo **jogo); 
void pegaGrid(Jogo *jogo, int grid[5][5]); 
void mudaGrid(Jogo *jogo, int grid[5][5]); 
void novoJogo(Jogo *jogo); 
void movimentaTabuleiro(Jogo *jogo, int x, int y); 
void pegaSolucao(Jogo *jogo, char solucao[100]); 

#endif
