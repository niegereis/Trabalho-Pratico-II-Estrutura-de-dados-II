#include "../intercalacaoBalanceadaFM1/intercalacaoBalanceadaFM1.h"
#include "./teste1.c"
#include "./teste2.c"

int main() {
  printf("\nGERANDO...\n");

  EstrategiaDeIntercalacao estrategia = F2;
  Analise analise = AnaliseCriar();

  IntercalacaoBalanceada(estrategia, 1000, &analise);

  AnaliseImprime(analise);

  return 0;
}