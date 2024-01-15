#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "quickSortExterno.h"
#include "aluno.h"
#include "compartilhado.h"
#define ORD 100

int main(){
  FILE *a = fopen("PROVAO.TXT","r");
  FILE *o = fopen("output.txt", "w+");
  if(a == NULL){perror("error");}
  if(o == NULL){perror("error");}

  copiaArquivo(a, o, ORD);

  Contagem cont;
  inicializaContagem(&cont);
  OrdenarQS(&cont, o, ORD);

  fclose(a);
  fclose(o);
  return 0;
}