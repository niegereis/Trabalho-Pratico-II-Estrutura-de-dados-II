// #include "../intercalacaoBalanceadaFM1/intercalacaoBalanceadaFM1.h"

// int main() {

//   Fita *fitas = FM1GerarBlocos(500);
//   if (fitas == NULL) {
//     printf("Ocorreu um erro inesperado!");
//     return 0;
//   }

//   while (true) {

//     FM1FitaResetarArquivos(fitas);
//     bool x = FM1JuntarNaFitaDeSaida(fitas);
//     FM1RegerarFitas(fitas, ENTRADA);

//     if (x)
//       break;

//     FM1FitaResetarArquivos(fitas);
//     FM1EspalharBlocosDaSaida(fitas);
//     FM1RegerarFitas(fitas, SAIDA);
//   }

//   FM1FitaFecharArquivos(fitas);
//   return 0;
// }