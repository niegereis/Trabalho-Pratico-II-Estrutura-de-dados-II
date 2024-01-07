#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct aluno {
  char numeroInscricao[9];
  float nota;
  char estado[3];
  char cidade[51];
  char curso[31];
} Aluno;

typedef struct fM1Bloco {
  Aluno *alunos;
  int qtdItens;
  int qtdMax;

  // usado na fase de intercalacao
  int posicaoAtualNoBloco;
} FM1Bloco;

void imprimeAluno(Aluno *aluno) {
  printf("Inscrição: %s => Nota: %.1f, E: %s, C: %s, CURSO: %s.\n", aluno->numeroInscricao, aluno->nota, aluno->estado,
         aluno->cidade, aluno->curso);
}

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("Esse programa só serve para imprimir arquivos binários que só contenha alunos (n inscricao, nota, estado, cidade, "
           "curso)\n");
    printf("Uso ./executavel <arquivo_do_arquivo>\n");
    return 1;
  }

  FILE *file = fopen(argv[1], "rb");
  if (!file) {
    printf("Passe um caminho de um arquivo existente!\n");
    return 1;
  }

  FM1Bloco bloco;
  bool hasNext = true;
  do {

    hasNext = fread(&bloco, sizeof(FM1Bloco), 1, file) == 1;
    if (!hasNext)
      return 0;
    printf("\n===================================================================== Bloco "
           "QTD MAX: %d, QTD ITENS: %d =====================================================\n\n",
           bloco.qtdMax, bloco.qtdItens);
    Aluno *alunos = malloc(sizeof(Aluno) * bloco.qtdItens);
    fread(alunos, sizeof(Aluno), bloco.qtdItens, file);
    bloco.alunos = alunos;
    for (int i = 0; i < bloco.qtdItens; i++) {
      imprimeAluno(&bloco.alunos[i]);
    }
    printf("\n==================================================================================================================="
           "============================================\n");
  } while (hasNext);

  return 0;
}