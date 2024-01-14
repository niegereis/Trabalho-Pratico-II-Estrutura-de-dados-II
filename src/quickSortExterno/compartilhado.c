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

void copiaArquivo(FILE* in, FILE* out, int tam ){
  Aluno al;
  for(int i = 0; i < tam; i++){
    fseek(in, i*101, 0);
    al = AlunoLer(in);
    fseek(out, i*101, 0);
    alunoEscreve(out, al);
    fprintf(out," ");
    fprintf(out,"\n");
  }
}