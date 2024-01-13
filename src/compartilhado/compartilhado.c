#include "./compartilhado.h"

Fita FitaCriar(FILE *arquivo, bool ehSaida) {
  Fita f;
  f.arquivo = arquivo;
  f.ehSaida = ehSaida;
  return f;
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
};
