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
  for(int i = 1; i < 50; i++){if(aluno.cidade[i] == ' ' && aluno.cidade[i-1] == ' '){aluno.cidade[i-1] = '\0';}} //remove os espaços em branco
  fgetc(f);
  fgets(aluno.curso, 30, f);
  for(int i = 1; i < 30; i++){if(aluno.curso[i] == ' ' && aluno.curso[i-1] == ' '){aluno.curso[i-1] = '\0';}} //remove os espaços em branco


  return aluno;
}

int main(){
  FILE *a = fopen("PROVAO.TXT","r");
  if(a == NULL){perror("error");}
  Aluno al;

  fseek(a, -100, 2);

  al = AlunoLer(a);
  AlunoImprimeCompleto(&al);


  fclose(a);
  return 0;
}