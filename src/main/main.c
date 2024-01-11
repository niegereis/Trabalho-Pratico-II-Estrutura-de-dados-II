#include "../intercalacaoBalanceadaFM1/intercalacaoBalanceadaFM1.h"
#include "./teste1.c"
#include "./teste2.c"

int main() {
  printf("GERANDO...\n\n");

  EstrategiaDeIntercalacao estrategia = F2;

  Fita *fitas = FM1GerarBlocos(50000, estrategia);
  if (fitas == NULL) {
    printf("Ocorreu um erro inesperado!");
    return 0;
  }
  printf("\nINTERCALANDO...\n\n");

  bool finalizou;

  if (estrategia == FM1) {
    while (true) {

      FM1FitaResetarArquivos(fitas);
      bool x = FM1JuntarNaFitaDeSaida(fitas);
      FitaRegerarFitas(fitas, FITA_DE_ENTRADA);

      if (x)
        break;

      FM1FitaResetarArquivos(fitas);
      FM1EspalharBlocosDaSaida(fitas);
      FitaRegerarFitas(fitas, FITA_DE_SAIDA);
    }
  } else {
    while (true) {
      FM1FitaResetarArquivos(fitas);
      finalizou = FM2JuntarNaFitaDe(fitas, FITA_DE_SAIDA);
      FitaRegerarFitas(fitas, FITA_DE_ENTRADA);

      if (finalizou)
        break;

      FM1FitaResetarArquivos(fitas);
      finalizou = FM2JuntarNaFitaDe(fitas, FITA_DE_ENTRADA);
      FitaRegerarFitas(fitas, FITA_DE_SAIDA);

      if (finalizou)
        break;
    }
  }

  FM1FitaFecharArquivos(fitas);
  return 0;
}