#include "../lib/aluno.h"
#include <string.h>

Aluno AlunoCriar(char *numeroInscricao, double nota, char *estado, char *cidade, char *curso) {
  Aluno aluno;
  strncpy(aluno.numeroInscricao, numeroInscricao, 9);
  aluno.nota = nota;
  strncpy(aluno.estado, estado, 3);
  strncpy(aluno.cidade, cidade, 51);
  strncpy(aluno.curso, curso, 31);
  return aluno;
}

void AlunoImprime(Aluno *aluno) { printf("Inscrição: %s => Nota: %.1f", aluno->numeroInscricao, aluno->nota); }
void AlunoImprimeCompleto(Aluno *al) {
  printf("%s %05.1f %s %-49s %s\n", al->numeroInscricao, al->nota, al->estado, al->cidade, al->curso);
}

Aluno AlunoLer(FILE *f) {
  Aluno aluno;

  fscanf(f, "%s %f %s ", aluno.numeroInscricao, &aluno.nota, aluno.estado);
  fgets(aluno.cidade, 50, f);
  for (int i = 1; i < 50; i++) {
    if (aluno.cidade[i] == ' ' && aluno.cidade[i - 1] == ' ') {
      aluno.cidade[i - 1] = '\0';
    }
  } // remove os espaços em branco
  fgetc(f);
  fgets(aluno.curso, 30, f);
  for (int i = 1; i < 30; i++) {
    if (aluno.curso[i] == ' ' && aluno.curso[i - 1] == ' ') {
      aluno.curso[i - 1] = '\0';
    }
  } // remove os espaços em branco

  return aluno;
}

bool AlunoLerViaArquivoBinario(FILE *f, Aluno *alunoLido) { return fread(alunoLido, sizeof(Aluno), 1, f) == 1; }

void AlunoEscreverEmArquivoBin(Aluno *aluno, FILE *arquivoBinario) { fwrite(aluno, sizeof(Aluno), 1, arquivoBinario); }

void AlunoEscreve(FILE *arquivo, Aluno al) {
  fprintf(arquivo, "%s %05.1f %s %-49s %-31s", al.numeroInscricao, al.nota, al.estado, al.cidade, al.curso);
}

void AlunoEscreveSeparadosPorNovaLinha(FILE *arquivo, Aluno *aluno) {

  fprintf(arquivo, "%s %05.1f %s %-49s %-31s\n", aluno->numeroInscricao, aluno->nota, aluno->estado, aluno->cidade, aluno->curso);
}
