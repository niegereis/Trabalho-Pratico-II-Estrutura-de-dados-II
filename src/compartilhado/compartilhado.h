#ifndef COMPARTILHADO_H
#define COMPARTILHADO_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Contagem {
  int comp;           // numero de comparacoes
  int transf;         // numero de transferencias
  int transfEscrita;  // numero de transferencias de escrita
  int transfLeitura;  // numero de transferencias de leitura
  int tempo;          // tempo de execucao
} Contagem;

#endif  // !COMPARTILHADO_H