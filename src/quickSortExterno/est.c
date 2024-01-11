#include "../aluno/aluno.h"
#include<stdio.h>

void AlunoImprimeCompleto(Aluno *aluno) {
  printf("Inscrição: %s => Nota: %.1f, E: %s, C: %s, CURSO: %s.", aluno->numeroInscricao, aluno->nota, aluno->estado,
         aluno->cidade, aluno->curso);
}

Aluno AlunoLer(FILE *f) {
  Aluno aluno;

  fscanf(f, "%s %f %s ", aluno.numeroInscricao, &aluno.nota, aluno.estado);
  fgets(aluno.cidade, 50, f);
  fgetc(f);
  fgets(aluno.curso, 30, f);

  return aluno;
}

int main(){
  FILE *a = fopen("PROVAO.TXT","r");
  Aluno al;

  fseek(a, -100, 2);

  al = AlunoLer(a);
  AlunoImprimeCompleto(&al);


  fclose(a);
  return 0;
}