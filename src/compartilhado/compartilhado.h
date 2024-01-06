#ifndef COMPARTILHADO_H
#define COMPARTILHADO_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fita {
  FILE *arquivo;
  bool ehSaida;
} Fita;

typedef struct contagem {
  int comp;          // numero de comparacoes
  int transf;        // numero de transferencias
  int transfEscrita; // numero de transferencias de escrita
  int transfLeitura; // numero de transferencias de leitura
  int tempo;         // tempo de execucao
} Contagem;

Fita FitaCriar(FILE *arquivo, bool ehSaida);

#endif // !COMPARTILHADO_H