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

typedef struct fM1Bloco {
  Aluno *alunos;
  int qtdItens;
  int qtdMax;

  // usado na fase de intercalacao
  int posicaoAtualNoBloco;
} FM1Bloco;

typedef enum tipoDeFita { SAIDA, ENTRADA } TipoDeFita;

Fita *FM1GerarBlocos(int qtdLinhas);

FM1Bloco FM1BlocoCriar(int qtdMax);
void FM1BlocoImprimir(FM1Bloco *bloco);
void FM1BlocoEscreverEmFita(Fita *fita, FM1Bloco *bloco);
bool FM1BlocoInserirAluno(FM1Bloco *bloco, Aluno *a);
bool FM1JuntarNaFitaDeSaida(Fita *fitas);
int FM1ObterFitaDeSaida(Fita *fitas);
char *FitaObterCaminhoPelaPosicao(int p);
bool FM1FinalizouOProcesso(Fita *fitas);
bool FM1BlocoLerViaArquivoBinario(FILE *arquivo, FM1Bloco *bloco);
void FM1FitaFecharArquivos(Fita *fitas);
void FM1FitaResetarArquivos(Fita *fitas);
void FM1EspalharBlocosDaSaida(Fita *fitas);
void FM1RegerarFitas(Fita *fitas, TipoDeFita tipo);

#endif // INTERCALACAOBALANCEADAFM1_H