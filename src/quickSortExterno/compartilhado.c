#include "compartilhado.h"

Fita FitaCriar(FILE *arquivo, bool ehSaida) {
  Fita f;
  f.arquivo = arquivo;
  f.ehSaida = ehSaida;
  return f;
}

void inicializaContagem(Contagem* cont){
  cont->comp = 0;          // numero de comparacoes
  cont->transf = 0;        // numero de transferencias
  cont->transfEscrita = 0; // numero de transferencias de escrita
  cont->transfLeitura = 0; // numero de transferencias de leitura
  cont->tempo = 0;  
}