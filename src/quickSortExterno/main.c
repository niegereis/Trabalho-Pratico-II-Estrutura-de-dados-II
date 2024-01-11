#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "quickSortExterno.h"
#include "../aluno/aluno.h"

int main(){
   void leArquivoEscreveArquivo(Aluno *aluno, File* arquivo, int quantidade){ /// terminar isso aq depois
    FILE *arquivo = fopen("PROVAO.TXT", "r");
    
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    int i;
    for (i = 0; i < 471705; i++){
       *aluno = AlunoLer(arquivo);

    }
    fclose(arquivo);
}
}