#ifndef COMPARTILHADO_H
#define COMPARTILHADO_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __USE_POSIX199309 1
#include <time.h>

#define DEBUG false

typedef struct fita {
  FILE *arquivo;
  bool ehSaida;
} Fita;

typedef struct contagem {
  int comparacoes;   // numero de comparacoes
  int transf;        // numero de transferencias
  int transfEscrita; // numero de transferencias de escrita
  int transfLeitura; // numero de transferencias de leitura
  int tempo;         // tempo de execucao
} Contagem;

typedef struct analise {
  int comparacoes;
  int transferenciasEscrita, transferenciasLeitura;
  long long int tempoTotal;
} Analise;

Fita FitaCriar(FILE *arquivo, bool ehSaida);

Analise AnaliseCriar();
void AnaliseImprime(Analise analise);
void AnaliseDefinirTempoPeloInicioEFim(Analise *analise, struct timespec inicio, struct timespec fim);

#endif // !COMPARTILHADO_H