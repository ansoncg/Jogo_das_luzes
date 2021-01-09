#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "logica.h"

typedef struct tabuleiro {
    int grid[5][5];
} Tabuleiro;

struct jogo {
    Tabuleiro tabuleiro;
    int solucao[5];
};

bool ehPossivel(Tabuleiro tabuleiro) {
    int soma, tabuleiroVet[25];
    char c, solucaoPossivel[2][20] = {
        "BCDFHJKLNOPRTVWX",
        "ABDEKLNOUVXY"
    };

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            tabuleiroVet[5*i + j] = tabuleiro.grid[j][i];
    for (int i = 0, j; i < 2; i++) {
        soma = j = 0;
        while ((c = solucaoPossivel[i][j++]) != '\0')
            soma += tabuleiroVet[c - 'A'];
        if (soma % 2)
            return false;
    }
    return true;
}


void mudaEstado(int *estado) {
   (*estado) = ((*estado) + 1)  % 2;
   return;
}

void solucionaJogo(const Tabuleiro tabuleiro, int solucaoJogo[5]) {
    char c, solucaoSistema[5][20] = { // Solução do sistema linear para 5x5.
        "BCDHJNOT",
        "ABDEGMNOS",
        "ACDEFHIMNPQRSTV",
        "ADFGHJKLNOPQRTUX",
        "BCEFKMORTUV"
    };
    int tabuleiroVet[25];

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            tabuleiroVet[5*i + j] = tabuleiro.grid[j][i];
    for (int i = 0, j; i < 5; i++) {
        solucaoJogo[i] = j = 0;
        while ((c = solucaoSistema[i][j++]) != '\0') 
            solucaoJogo[i] += tabuleiroVet[c - 'A']; 
        solucaoJogo[i] %= 2;
    }
    return;
}

void movimentaTabuleiro(Jogo *jogo, int x, int y) {
    if (x >= 0 && x < 5 && y >= 0 && y < 5) {
       mudaEstado(&jogo->tabuleiro.grid[x][y]);
       if (x > 0)
           mudaEstado(&jogo->tabuleiro.grid[x - 1][y]);
       if (x < 4)
           mudaEstado(&jogo->tabuleiro.grid[x + 1][y]);
       if (y > 0)
           mudaEstado(&jogo->tabuleiro.grid[x][y - 1]);
       if (y < 4)
           mudaEstado(&jogo->tabuleiro.grid[x][y + 1]);
    }
    return;
}

void pegaGrid(Jogo *jogo, int grid[5][5]) {
    memcpy(grid, jogo->tabuleiro.grid, sizeof(int)*25);
    return;
}

void mudaGrid(Jogo *jogo, int grid[5][5]) {
    memcpy(jogo->tabuleiro.grid, grid, sizeof(int)*25);
    return;
}

void novoJogo(Jogo *jogo) {
    srand((unsigned int) time(NULL));
    do {
        for (int i = 0; i < 5; i++)
           for (int j = 0; j < 5; j++)
               jogo->tabuleiro.grid[i][j] = rand() % 2; 
    } while (!ehPossivel(jogo->tabuleiro));
    return;
}

void criaJogo(Jogo **jogo) {
    *jogo = calloc(1, sizeof(Jogo));
    return;
}

void destroiJogo(Jogo **jogo) {
    free(*jogo);
    *jogo = NULL;
    return;
}

void pegaSolucao(Jogo *jogo, char solucao[100]) {
    memset(jogo->solucao, 0, sizeof(int)*5); 
    solucionaJogo(jogo->tabuleiro, jogo->solucao);
    sprintf(solucao, "%d %d %d %d %d", jogo->solucao[0], jogo->solucao[1], 
            jogo->solucao[2], jogo->solucao[3], jogo->solucao[4]);
    return;
}
