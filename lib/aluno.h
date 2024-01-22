#ifndef ALUNO_H
#define ALUNO_H

#include <stdbool.h>
#include <stdio.h>

#define ALUNO_LINHA 101

typedef struct aluno {
  char numeroInscricao[9];
  float nota;
  char estado[3];
  char cidade[51];
  char curso[31];
} Aluno;

Aluno AlunoCriar(char *numeroInscricao, double nota, char *estado, char *cidade, char *curso);
void AlunoImprime(Aluno *aluno);
void AlunoImprimeCompleto(Aluno *aluno);
Aluno AlunoLer(FILE *f);
bool AlunoLerViaArquivoBinario(FILE *f, Aluno *alunoLido);
void AlunoEscreverEmArquivoBin(Aluno *aluno, FILE *arquivoBinario);
void AlunoEscreve(FILE *arquivo, Aluno al);
void AlunoEscreveSeparadosPorNovaLinha(FILE *arquivo, Aluno *aluno);

#endif // ALUNO_H