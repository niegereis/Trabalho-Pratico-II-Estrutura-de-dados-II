#include "compartilhado.h"
#include <stdlib.h> 
#include <string.h>
#define TAMMAX 471705

int main(int argc, char** argv){
    if ((argc != 4 && argc != 5) || (atoi(argv[1]) < 1 || atoi(argv[1]) > 3) || (atoi(argv[2]) < 1 || atoi(argv[2]) > TAMMAX) || (atoi(argv[3]) < 1 || atoi(argv[3]) > 3)) {
        printf("Argumentos inválidos\n");
        printf("./output/main <metodo> <quantidade> <situacao>");
        printf("\n<metodo>\n");
        printf("\t1 - Intercalação balanceada de vários caminhos (2f fitas) \n");
        printf("\t2 - Intercalação balanceada de vários caminhos (f+1 fitas)\n");
        printf("\t3 - Quicksort externo\n");
        printf("<quantidade>\n");
        printf("\tvalor inteiro entre 1 e 471705\n");
        printf("<situacao>\n");
        printf("\t1 - crescente\n");
        printf("\t2 - decrescente\n");
        printf("\t3 - desordenado\n");
        exit(1);
    }

    int metodo, quantidade, situacao, p = 0;
    metodo = atoi(argv[1]);
    quantidade = atoi(argv[2]);
    situacao = atoi(argv[3]);

    if(argc == 5 && (strcmp(argv[4], "-P") || strcmp(argv[4], "-p")))
        p = 1;


    FILE *output = fopen("./arquivos/output.txt", "w+");
    if(output == NULL){
        perror("error(output file):");
        exit(1);
    }

    copiaArquivo(output, situacao, quantidade);
    fclose(output);

    Analise analise;
    analise = ordenaMain(quantidade, metodo);
    if(p)
        imprimeTerminal(quantidade);
    AnaliseImprime(analise);


    return 0;
}