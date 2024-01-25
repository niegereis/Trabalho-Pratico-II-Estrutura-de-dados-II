#include "../lib/compartilhado.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TAMMAX 471705

int main(int argc, char **argv) {
  if ((argc != 4 && argc != 5) || (atoi(argv[1]) < 1 || atoi(argv[1]) > 3) || (atoi(argv[2]) < 1 || atoi(argv[2]) > TAMMAX) ||
      (atoi(argv[3]) < 1 || atoi(argv[3]) > 3)) {
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

  int metodo, quantidade, situacao;
  bool p = false;//váriavel para impressão no terminal

  metodo = atoi(argv[1]);
  quantidade = atoi(argv[2]);
  situacao = atoi(argv[3]);

  FILE *output = fopen("./arquivos/output.txt", "w+");
  if (output == NULL) {
    perror("error(output file):");
    exit(1);
  }

  if (argc == 5 && (strcmp(argv[4], "-P") || strcmp(argv[4], "-p")))
    p = true;

  //copia o arquivo selecionado pelo usuário para o output.txt, onde será ordenado
  copiaArquivo(output, situacao, quantidade);
  fclose(output);

  if (p){
    printf("\nAntes da ordenação:\n");
    imprimeTerminal(quantidade);
  }

  Analise analise;
  analise = ordenaMain(quantidade, metodo);
  if (p){
    printf("\nDepois da ordenação:\n");
    imprimeTerminal(quantidade);
  }
  AnaliseImprime(analise);

  return 0;
}