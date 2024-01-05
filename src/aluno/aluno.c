

#include "aluno.h"

Aluno AlunoCriar(long int numeroInscricao, double nota, char* estado,
                 char* cidade, char* curso) {
  Aluno aluno;
  aluno.numeroInscricao = numeroInscricao;
  aluno.nota = nota;
  strncpy(aluno.estado, estado, 3);
  strncpy(aluno.cidade, cidade, 51);
  strncpy(aluno.curso, curso, 31);
  return aluno;
}

void AlunoImprime(Aluno* aluno) {
  printf("Inscrição: %ld => Nota: %.1f", aluno->numeroInscricao, aluno->nota);
}

Aluno AlunoLer(FILE* f) {
  Aluno aluno;

  fscanf(f, "%ld %f %s %s %s", &aluno.numeroInscricao, &aluno.nota,
         aluno.estado, aluno.cidade, aluno.curso);

  return aluno;
}