#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "quickSortExterno.h"
#include "aluno.h"
#include "compartilhado.h"

int main(){
  FILE *a = fopen("PROVAO.TXT","r+");
  if(a == NULL){perror("error");}
  
  Contagem cont;
  inicializaContagem(&cont);
  OrdenarQS(&cont, a, 10);

  fclose(a);
  return 0;
}