#include "../lib/quickSortExterno.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void leSup(FILE **arqLEs, Aluno *ultLido, int *ls, short *ondeLer) {
  fseek(*arqLEs, (*ls - 1) * ALUNO_LINHA, SEEK_SET);
  *ultLido = AlunoLer(*arqLEs);
  (*ls)--;
  *ondeLer = false;
}

void leInf(FILE **arqLEi, Aluno *ultLido, int *li, short *ondeLer) {
  fseek(*arqLEi, (*li - 1) * ALUNO_LINHA, SEEK_SET);
  *ultLido = AlunoLer(*arqLEi);
  (*li)++;
  *ondeLer = true;
}

void inserirArea(TipoArea *area, Aluno aluno) {
  int i = area->n;
  while (i > 0 && aluno.nota < area->r[i - 1].nota) {
    area->r[i] = area->r[i - 1];
    i--;
  }
  area->r[i] = aluno;
  area->n++;
}

void escreveMax(FILE **arqLEs, Aluno R, int *es) { // Escreve o maior elemento da área no final do arquivo de saída
  // Escreve o maior elemento da área no final do arquivo de saída
  fseek(*arqLEs, (*es - 1) * ALUNO_LINHA, SEEK_SET);
  AlunoEscreve(*arqLEs, R);
  (*es)--;
}

void escreveMin(FILE **arqLEi, Aluno R, int *Ei) { // Escreve o menor elemento da área no início do arquivo de saída
  // Escreve o menor elemento da área no início do arquivo de saída
  fseek(*arqLEi, (*Ei - 1) * ALUNO_LINHA, SEEK_SET);
  AlunoEscreve(*arqLEi, R);
  (*Ei)++;
}

// ---------------------------- RETIRANDO O ULTIMO ITEM DA AREA ----------------------------

void retiraMax(TipoArea *area, Aluno *aluno) {
  *aluno = area->r[area->n - 1];
  area->n--;
}

// -----------------------------------------------------------------------------------------

// ---------------------------- RETIRANDO O PRIMEIRO ITEM DA AREA ----------------------------

void retiraMin(TipoArea *area, Aluno *aluno) {
  *aluno = area->r[0];
  area->n--;
  for (int i = 0; i < area->n; i++) {
    area->r[i] = area->r[i + 1];
  }
}

// ------------------------------------------------------------------------------------------

void particao(FILE **arqEi, FILE **arqLEs, TipoArea area, int esq, int dir, int *i, int *j, Analise *analise) {
  int ls = dir, es = dir, li = esq, Ei = esq;
  float lInf = INT_MIN, lSup = INT_MAX;
  short ondeLer = true;
  Aluno ultLido, R;

  *i = esq - 1;
  *j = dir + 1;

  analise->comparacoes++;
  while (ls >= li) {
    analise->comparacoes++;
    if (area.n < TAMANHOAREA - 1) { // verificar se tem parte do pivo vazio
      if (ondeLer) {
        analise->transferenciasLeitura++;
        leSup(arqLEs, &ultLido, &ls, &ondeLer);
      } else {
        analise->transferenciasLeitura++;
        leInf(arqEi, &ultLido, &li, &ondeLer);
      }
      inserirArea(&area, ultLido);
      continue;
    }
    analise->comparacoes++;
    if (ls == es) { // se o ls for igual ao es, então o ls é o maior elemento da área
      analise->transferenciasLeitura++;
      leSup(arqLEs, &ultLido, &ls, &ondeLer);
    } else if (li == Ei) {
      analise->comparacoes++;
      analise->transferenciasLeitura++;
      leInf(arqEi, &ultLido, &li, &ondeLer);
    } else if (ondeLer) {
      analise->comparacoes += 2;
      analise->transferenciasLeitura++;
      leSup(arqLEs, &ultLido, &ls, &ondeLer);
    } else {
      analise->comparacoes += 2;
      analise->transferenciasLeitura++;
      leInf(arqEi, &ultLido, &li, &ondeLer);
    }

    analise->comparacoes++;
    if (ultLido.nota > lSup) { // se o ultimo lido for maior que o limite superior
      analise->transferenciasEscrita++;
      *j = es;
      escreveMax(arqLEs, ultLido, &es);
      continue;
    }

    analise->comparacoes++;
    if (ultLido.nota < lInf) {
      analise->transferenciasEscrita++;
      *i = Ei;
      escreveMin(arqEi, ultLido, &Ei);
      continue;
    }

    // colocar a funçao de inserir pivo
    // ------------------------------------------------------

    inserirArea(&area, ultLido);

    // ------------------------------------------------------
    analise->comparacoes++;
    if (Ei - esq < dir - es) { // retira o aluno com maior nota
                               // retirar o maior item do pivo
      analise->transferenciasEscrita++;
      retiraMin(&area, &R);
      escreveMin(arqEi, R, &Ei);
      lInf = R.nota;
    } else { // retira o aluno com menor nota
             // retira menor item do pivo
      analise->transferenciasEscrita++;
      retiraMax(&area, &R);
      escreveMax(arqLEs, R, &es);
      lSup = R.nota;
    }
  }

  analise->comparacoes++;
  while (Ei <= es) {
    analise->comparacoes++;           // numero de comparações
    analise->transferenciasEscrita++; // numero de transferencias de escrita
    retiraMin(&area, &R);
    escreveMin(arqEi, R, &Ei);
  }
}

void quickSortExterno(FILE **arqLEi, FILE **arqLEs, int esq, int dir, Analise *analise) { // função recusiva
  int i, j;
  TipoArea area;
  area.n = 0;
  if (dir - esq < 1)
    return;
  particao(arqLEi, arqLEs, area, esq, dir, &i, &j, analise);

  if (i - esq < dir - j) {
    quickSortExterno(arqLEi, arqLEs, esq, i, analise);
    quickSortExterno(arqLEi, arqLEs, j, dir, analise);
  } else {
    quickSortExterno(arqLEi, arqLEs, j, dir, analise);
    quickSortExterno(arqLEi, arqLEs, esq, i, analise);
  }
}

void ordenarQS(Analise *analise, int quantidade) { // FUNÇÃO principal
  struct timespec inicio, fim;
  FILE *arquivo;

  arquivo = fopen("./arquivos/output.txt", "r+");
  if (arquivo == NULL) {
    perror("error: ");
    exit(1);
  }

  clock_gettime(1, &inicio);

  quickSortExterno(&arquivo, &arquivo, 1, quantidade, analise);

  clock_gettime(1, &fim);

  AnaliseDefinirTempoPeloInicioEFim(analise, inicio, fim);
  fclose(arquivo);
}
