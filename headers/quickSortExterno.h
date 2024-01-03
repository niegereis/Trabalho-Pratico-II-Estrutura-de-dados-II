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

typedef long TipoChave;

typedef struct TipoPagina* TipoApontador;

// struct de tipoArea

typedef struct TipoArea { // Tipo usado para representar a área de memória interna
    TipoRegistro *r; // Vetor de registros
    int n; // Número de registros armazenados
} TipoArea;

#endif