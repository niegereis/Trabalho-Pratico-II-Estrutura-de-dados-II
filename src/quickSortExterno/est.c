// #include "../aluno/aluno.h"
// #include<stdio.h>

// void AlunoImprimeCompleto2(Aluno al) {
//   printf("%s 0%.1f %s %-49s %-31s\n", al.numeroInscricao, al.nota, al.estado, al.cidade, al.curso);
// }

// void alunoEscreve(FILE *arquivo, Aluno al) {
//   //FILE* prox = arquivo;
//   fprintf(arquivo,"%s 0%.1f %s %-49s %-31s", al.numeroInscricao, al.nota, al.estado, al.cidade, al.curso);
//   // char x = fgetc(arquivo);
//   // printf("EEEEEEEEEEEEEEEEE%cee\n", x);
//   // fseek(arquivo, -1, 1);
//   //fseek(arquivo, -101, 1);
//   //arquivo = prox;
// }

// Aluno AlunoLer(FILE *f) {
//   Aluno aluno;

//   fscanf(f, "%s %f %s ", aluno.numeroInscricao, &aluno.nota, aluno.estado);
//   fgets(aluno.cidade, 50, f);
//   for(int i = 1; i < 50; i++){if(aluno.cidade[i] == ' ' && aluno.cidade[i-1] == ' '){aluno.cidade[i-1] = '\0';}} //remove os espaços em branco
//   fgetc(f);
//   fgets(aluno.curso, 30, f);
//   for(int i = 1; i < 30; i++){if(aluno.curso[i] == ' ' && aluno.curso[i-1] == ' '){aluno.curso[i-1] = '\0';}} //remove os espaços em branco

//   char x = fgetc(f);
//   printf("LL%cll\n", x);

//   fseek(f, -1, 1);

//   return aluno;
// }

// // void alunoEscreve(FILE *f, Aluno aluno){
// //   fprintf
// // }

// int main(){
//   FILE *a = fopen("PROVAO.TXT","r+");
//   if(a == NULL){perror("error");}
//   Aluno al;
//   Aluno al2;
//   Aluno al3;

//   fseek(a, 200*101, 0);

//   al = AlunoLer(a);
//   al2 = AlunoLer(a);
//   al3 = AlunoLer(a);
//   AlunoImprimeCompleto2(al);
//   AlunoImprimeCompleto2(al2);
//   AlunoImprimeCompleto2(al3);
//   fseek(a, 0*101, 0);
//   alunoEscreve(a, al);
//   fseek(a, 1*101, 0);
//   alunoEscreve(a, al);
//   fseek(a, 2*101, 0);
//   alunoEscreve(a, al2);

//   fclose(a);
//   return 0;
// }