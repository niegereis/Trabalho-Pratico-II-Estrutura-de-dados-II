#ifndef QUICKSORT_EXTERNO_H
#define QUICKSORT_EXTERNO_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../compartilhado/compartilhado.h"

typedef long int TipoChave;

typedef struct TipoRegistro {
  TipoChave chave;
} TipoRegistro;

typedef struct TipoPagina *TipoApontador;

// struct de tipoArea
typedef struct TipoArea {  // Tipo usado para representar a área de memória
                           // interna
  TipoRegistro *r;         // Vetor de registros
  int n;                   // Número de registros armazenados
} TipoArea;

void QuickInicializaArea(TipoArea *, Contagem *);
void QuickSortExterno(FILE **, FILE **, FILE **, int, int, Contagem *);
void QuickLeSup(FILE **, TipoRegistro *, int *, short *);
void QuickLeInf(FILE **, TipoRegistro *, int *, short *);
void QuickInserirArea(TipoArea *, TipoRegistro *, int *);
void QuickEscreveMax(FILE **, TipoRegistro, int *);
void QuickEscreveMin(FILE **, TipoRegistro, int *);
void QuickRetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea);
void QuickRetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea);
void QuickParticao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area,
                   int Esq, int Dir, int *i, int *j, TipoArea area);
#endif