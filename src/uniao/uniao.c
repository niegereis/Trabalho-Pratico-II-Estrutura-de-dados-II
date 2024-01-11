#include "../lib/uniao.h"

void contagemInicia(Contagem* contagem) {
  int comp = 0;          // numero de comparacoes
  int transfEscrita = 0; // numero de transferencias de escrita
  int transfLeitura = 0; // numero de transferencias de leitura
  int tempo = 0;   
}

void imprimeAnalise(Contagem con) {
  printf(
      "\nComparações: %d\n\tTransferências(Leitura): %d\n\tTransferências(Escrita): %d\n\tTempo(em "
      "nano segundos): %lld",
      con.comp, con.transfEscrita, con.transfLeitura, con.tempo);
}

void ordenacao(int metodo, int situacao, int quantidade, Contagem* contagem) {
  contagemInicia(contagem);
  FILE* arquivo;

  switch (situacao) {
    case 1:
      arquivo = fopen("./arquivos/crescente.txt", "r+");
      break;

    case 2:
      arquivo = fopen("./arquivos/decrescente.txt", "r+");
      break;

    case 3:
      arquivo = fopen("./arquivos/aleatorio.txt", "r+");
      break;
  }

  if (arquivo == NULL) {
    perror("Não foi possível abrir o arquivo.\n");
    return;
  }

  switch (metodo) {
    case 1:  // 2f fitas
      encontrado = acessoSequencialIndexado();
      break;

    case 2:  // f+1 fitas
      encontrado = arvoreBinaria();
      break;

    case 3:  // quicksort externo
      encontrado = arvoreB();
      break;

    default:
      printf("Invalido\n");
      return;
  }

  fclose(arquivo);


}
