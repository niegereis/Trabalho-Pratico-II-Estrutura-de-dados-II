#ifndef INTERCALACAOBALANCEADA_H
#define INTERCALACAOBALANCEADA_H

#include "../lib/aluno.h"
#include "../lib/compartilhado.h"
#include "../lib/heap.h"

#include <dirent.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define QTD_FITAS_FM1 20
#define QTD_FITAS_2F 20

#define HEAP_TYPE HEAP_MAX

typedef struct fita {
  FILE *arquivo;
  bool ehSaida;
} Fita;

typedef struct bloco {
  Aluno *alunos;
  int qtdItens;
  int qtdMax;

  // usado na fase de intercalacao
  int posicaoAtualNoBloco;
} Bloco;

typedef enum estrategiaDeIntercalacao { F2, FM1 } EstrategiaDeIntercalacao;
typedef enum tipoDeFita { FITA_DE_SAIDA, FITA_DE_ENTRADA } TipoDeFita;

// FUNÇÕES - FITAS

Fita FitaCria(FILE *arquivo, bool ehSaida);
Fita *FitaGerarBlocos(int qtdLinhas, EstrategiaDeIntercalacao estrategia, Analise *analise);
void FitaFecharArquivos(Fita *fitas, EstrategiaDeIntercalacao estrategia);
void FitaRegerarFitas(Fita *fitas, TipoDeFita tipo, EstrategiaDeIntercalacao estrategia);
Fita *FitaGerarFitas(int qtd, EstrategiaDeIntercalacao estrategia);
int FitaObterNumeroDeFitaDeEntradas(Fita *f, EstrategiaDeIntercalacao estrategia);
void FitaResetarArquivos(Fita *fitas, EstrategiaDeIntercalacao estrategia);
int FitaObterQuantidadeDeFitas(EstrategiaDeIntercalacao estrategia);

// FUNÇÕES - BLOCO

Bloco BlocoCria(int qtdMax);
void BlocoImprimir(Bloco *bloco);
int BlocoEscreverEmFita(Fita *fita, Bloco *bloco);
bool BlocoInserirAluno(Bloco *bloco, Aluno *a);
bool BlocoLerViaArquivoBinario(FILE *arquivo, Bloco *bloco);

// FUNÇÕES - F + 1 fitas

int FM1ObterFitaDeSaida(Fita *fitas);
void FM1DefinirFitaSaida(Fita *fitas);
bool FM1JuntarNaFitaDeSaida(Fita *fitas, Analise *analise);
bool FM1FinalizouOProcesso(Fita *fitas);
void FM1EspalharBlocosDaSaida(Fita *fitas, Analise *analise);

// FUNÇÕES - 2 F fitas

void F2DefinirFitasDeSaida(Fita *fitas);
bool F2JuntarNaFitaDe(Fita *fitas, TipoDeFita tipo, Analise *analise);

// FUNÇÕES - Intercalação

void IntercalacaoBalanceada(EstrategiaDeIntercalacao estrategia, int linhasALer, Analise *analise);

#endif // INTERCALACAOBALANCEADA_H