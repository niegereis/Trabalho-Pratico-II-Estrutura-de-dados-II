#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>


#ifndef QUICKSORT_EXTERNO_H
#define QUICKSORT_EXTERNO_H


typedef struct TipoRegistro {
    TipoChave chave;
} TipoRegistro;

typedef struct Contagem {
    int comp; // numero de comparacoes
    int transf; // numero de transferencias
    int transfEscrita; // numero de transferencias de escrita
    int transfLeitura; // numero de transferencias de leitura
    int tempo; // tempo de execucao
} Contagem;

typedef struct Aluno {
    int numeroDeInscrição;
    double nota;
    char estado[2];
    char cidade[51];
    char curso[31];

} Aluno;

typedef long TipoChave;

typedef struct TipoPagina* TipoApontador;

// struct de tipoArea

typedef struct TipoArea { // Tipo usado para representar a área de memória interna
    TipoRegistro *r; // Vetor de registros
    int n; // Número de registros armazenados
} TipoArea;


void inicializaArea(TipoArea *, Contagem *);
void QuickSortExterno(FILE **, FILE **, FILE **, int , int , Contagem *);
void LeSup(FILE **, TipoRegistro *, int *, short *);
void LeInf(FILE **, TipoRegistro *, int *, short *);
void inserirArea(TipoArea *, TipoRegistro *, int *);
void EscreveMax(FILE **, TipoRegistro , int *);
void EscreveMin(FILE **, TipoRegistro , int *);
void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea);
void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea);
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j, TipoArea area);
#endif