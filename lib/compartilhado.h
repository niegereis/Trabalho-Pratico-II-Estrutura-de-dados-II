#ifndef COMPARTILHADO_H
#define COMPARTILHADO_H

#include <time.h>
#include <stdio.h>

#define __USE_POSIX199309 1
#define DEBUG false

typedef struct analise {
  int comparacoes;
  int transferenciasEscrita, transferenciasLeitura;
  long long int tempoTotal;
} Analise;


Analise AnaliseCriar();
void copiaArquivo(FILE*, int, int);
void copiaArquivoBin(FILE*, int);
void AnaliseImprime(Analise analise);
void AnaliseDefinirTempoPeloInicioEFim(Analise *analise, struct timespec inicio, struct timespec fim);
Analise ordenaMain(int, int);

#endif // !COMPARTILHADO_H