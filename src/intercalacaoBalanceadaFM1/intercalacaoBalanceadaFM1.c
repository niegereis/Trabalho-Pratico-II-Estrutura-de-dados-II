#include "./intercalacaoBalanceadaFM1.h"

void FM1GerarBlocos() {
  FILE *file = fopen("../PROVAO.TXT", "r");

  Aluno a = AlunoLer(file);

  AlunoImprime(&a);

  return;
}