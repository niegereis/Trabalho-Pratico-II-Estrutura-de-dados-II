#include "../lib/compartilhado.h"
#include "../lib/aluno.h"
#include "../lib/heap.h"
#include "../lib/intercalacaoBalanceada.h"
#include "../lib/quickSortExterno.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

 // copia a quntidade de linhas definida pelo usuário do arquivo escolhido para output.txt
void copiaArquivo(FILE *out, int situacao, int tam) {
  FILE *in;
  switch (situacao) {
  case 1:
    in = fopen("./arquivos/crescente.txt", "r");
    break;

  case 2:
    in = fopen("./arquivos/decrescente.txt", "r");
    break;

  case 3:
    in = fopen("./arquivos/aleatorio.txt", "r");
    break;
  }
  if (in == NULL) {
    perror("error(input file):");
    exit(1);
  }

  Aluno al;
  for (int i = 0; i < tam; i++) {
    fseek(in, i * 101, 0);
    al = AlunoLer(in);
    fseek(out, i * 101, 0);
    AlunoEscreve(out, al);
    fprintf(out, " ");
    fprintf(out, "\n");
  }
  fclose(in);
}

void copiaArquivoBin(FILE *in, int tam) { // Lê fita final e a escreve no output.txt
  Aluno al;
  FILE *out = fopen("./arquivos/output.txt", "w+");
  fseek(in, 0, 0);
  for (int i = 0; i < tam; i++) {
    fseek(in, (i*(sizeof(Aluno)) + sizeof(Bloco)), 0);
    AlunoLerViaArquivoBinario(in, &al);
    fseek(out, i * 101, 0);
    AlunoEscreve(out, al);
    fprintf(out, " ");
    fprintf(out, "\n");
  }
  fclose(out);
}

Analise AnaliseCriar() {
  Analise analise;
  analise.comparacoes = 0;
  analise.tempoTotal = 0;
  analise.transferenciasEscrita = 0;
  analise.transferenciasLeitura = 0;
  return analise;
}

void AnaliseImprime(Analise analise) {
  printf("\n============ ANALISE ==============\n");
  printf("| Tempo Total:       %7lld (ms) |\n", analise.tempoTotal);
  printf("| Comparações:          %9d |\n", analise.comparacoes);
  printf("| Transferências Leitura: %7d |\n", analise.transferenciasLeitura);
  printf("| Transferências Escrita: %7d |\n", analise.transferenciasEscrita);
  printf("===================================\n");
}

void AnaliseDefinirTempoPeloInicioEFim(Analise *analise, struct timespec inicio, struct timespec fim) {
  analise->tempoTotal = ((fim.tv_sec - inicio.tv_sec) * 1e9 + (fim.tv_nsec - inicio.tv_nsec)) / 1e6;
}

//funçao chamada na main onde o arquivo é ordenado pelo método escolhido pelo usuário
Analise ordenaMain(int quantidade, int metodo) {
  Analise analise = AnaliseCriar();

  switch (metodo) {
  case (1): // 2f fitas
    IntercalacaoBalanceada(F2, quantidade, &analise);
    break;
  case (2): // f+1 fitas
    IntercalacaoBalanceada(FM1, quantidade, &analise);
    break;
  case (3): // QS externo
    ordenarQS(&analise, quantidade);
    break;
  }

  return analise;
}

//imprime os dados do arquivo output.txt para o terminal
void imprimeTerminal(int quantidade) {
  Aluno aluno;
  FILE *output = fopen("./arquivos/output.txt", "r");
  if (output == NULL) {
    perror("error(print file):");
    exit(1);
  }
  fseek(output, 0, 0);
  for (int i = 0; i < quantidade; i++) {
    aluno = AlunoLer(output);
    AlunoImprimeCompleto(&aluno);
  }
  fclose(output);
}