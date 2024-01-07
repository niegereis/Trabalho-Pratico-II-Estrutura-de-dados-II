#ifndef QUICKSORT_EXTERNO_H
#define QUICKSORT_EXTERNO_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../aluno/aluno.h"
#include "../compartilhado/compartilhado.h"

typedef long int TipoChave;

// typedef struct Aluno {
//  TipoChave chave;
//} Aluno;

typedef struct TipoPagina *TipoApontador;

// struct de tipoArea
typedef struct TipoArea { // Tipo usado para representar a área de memória
                          // interna
  Aluno *r;               // Vetor de registros
  int n;                  // Número de registros armazenados
} TipoArea;

void QuickInicializaArea(TipoArea *, Contagem *);
void QuickSortExterno(FILE **, FILE **, FILE **, int, int, Contagem *);
// void QuickLeSup(FILE **, Aluno *, int *, short *);
// void QuickLeInf(FILE **, Aluno *, int *, short *);
// void QuickInserirArea(TipoArea *, Aluno *, int *);
// void QuickEscreveMax(FILE **, Aluno, int *);
// void QuickEscreveMin(FILE **, Aluno, int *);
// void QuickRetiraMax(TipoArea *Area, Aluno *R, int *NRArea);
// void QuickRetiraMin(TipoArea *Area, Aluno *R, int *NRArea);
// void QuickParticao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j, TipoArea area);
#endif
