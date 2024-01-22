#ifndef QUICKSORT_EXTERNO_H
#define QUICKSORT_EXTERNO_H

#include<stdio.h>
#include "aluno.h"
#include "compartilhado.h"

#define TAMANHOAREA 10

typedef struct TipoArea { // Tipo usado para representar a área de memória
                          // interna
  Aluno r[TAMANHOAREA];   // Vetor de registros
  int n;                  // Número de registros armazenados
} TipoArea;

void ordenarQS(Analise*, int) ;

#endif
