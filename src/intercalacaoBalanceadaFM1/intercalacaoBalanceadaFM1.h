#ifndef INTERCALACAOBALANCEADAFM1_H
#define INTERCALACAOBALANCEADAFM1_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../aluno/aluno.h"
#include "../compartilhado/compartilhado.h"
#include "../heap/heap.h"

#define QTD_FITAS 10
#define HEAP_TYPE HEAP_MAX

typedef struct fM1Bloco {
  Aluno *alunos;
  int qtdItens;
  int qtdMax;

  // usado na fase de intercalacao
  int posicaoAtualNoBloco;
} FM1Bloco;

typedef enum tipoDeFita { FITA_DE_SAIDA, FITA_DE_ENTRADA } TipoDeFita;

typedef enum estrategiaDeIntercalacao { F2, FM1 } EstrategiaDeIntercalacao;

Fita *FM1GerarBlocos(int qtdLinhas, EstrategiaDeIntercalacao estrategia);

FM1Bloco FM1BlocoCriar(int qtdMax);
void FM1BlocoImprimir(FM1Bloco *bloco);
void FM1BlocoEscreverEmFita(Fita *fita, FM1Bloco *bloco);
bool FM1BlocoInserirAluno(FM1Bloco *bloco, Aluno *a);
bool FM1JuntarNaFitaDeSaida(Fita *fitas);
int FM1ObterFitaDeSaida(Fita *fitas);
char *FitaObterCaminhoPelaPosicaoFM1(int p);
bool FM1FinalizouOProcesso(Fita *fitas);
bool FM1BlocoLerViaArquivoBinario(FILE *arquivo, FM1Bloco *bloco);
void FM1FitaFecharArquivos(Fita *fitas);
void FM1FitaResetarArquivos(Fita *fitas);
void FM1EspalharBlocosDaSaida(Fita *fitas);
void FitaRegerarFitas(Fita *fitas, TipoDeFita tipo);

void F2DefinirFitasDeSaida(Fita *fitas);
bool FM2JuntarNaFitaDe(Fita *fitas, TipoDeFita);

#endif // INTERCALACAOBALANCEADAFM1_H