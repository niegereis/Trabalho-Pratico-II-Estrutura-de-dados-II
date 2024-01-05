

#ifndef ALUNO_H
#define ALUNO_H

#include <stdio.h>
#include <string.h>

typedef struct aluno {
  long int numeroInscricao;
  float nota;
  char estado[3];
  char cidade[51];
  char curso[31];
} Aluno;

Aluno AlunoCriar(long int numeroInscricao, double nota, char* estado,
                 char* cidade, char* curso);
void AlunoImprime(Aluno* aluno);
Aluno AlunoLer(FILE* f);

#endif  // ALUNO_H