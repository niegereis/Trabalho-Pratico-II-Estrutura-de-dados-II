#include "./quickSortExterno.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHOAREA 10
// Para o método de quickSort externo, deve ser considerada a existência de
// memória interna disponível para armazenar um vetor
//  de, no máximo, 10 registros
// A 2ª fase deste trabalho corresponde à análise experimental da complexidade
// de desempenho dos 3
/*métodos mencionados, considerando os seguintes quesitos:
1) número de transferências (leitura) de registros da memória externa para a
memória interna; 2) número de transferências (escrita) de registros da memória
interna para a memória externa; 3) número de comparações entre valores do campo
de ordenação dos registros; 4) tempo de execução (tempo do término de execução
menos o tempo do início de execução).*/

// Para a 2ª fase, experimentos devem ser realizados considerando o arquivo
// texto "PROVAO.TXT"
/*arquivo disponível na página da disciplina) que corresponde a uma base de
dados de notas de 471.705 alunos do Ensino Superior que fizeram o "Provão" em
2003 (último ano em que tal avaliação foi aplicada). Cada linha do arquivo
contém os seguintes dados relativos a um determinado aluno que fez o "Provão":
- número de inscrição do aluno (valor inteiro longo, ocupando as colunas 1 a 8
do arquivo texto);
- nota obtida pelo aluno (valor real entre 0.0 e 100.0, ocupando as colunas 10 a
14 do arquivo texto);
- estado do aluno (cadeia de 2 caracteres, ocupando as colunas 16 e 17 do
arquivo texto);
- cidade do aluno (cadeia de 50 caracteres, ocupando as colunas 19 a 68 do
arquivo texto);
- curso do aluno (cadeia de 30 caracteres, ocupando as colunas 70 a 99 do
arquivo texto).*/

// A area seria o pivo
// void QuickInicializaArea(TipoArea *Area, Contagem *C) {
//   Area->r = (TipoRegistro *)malloc(10 * sizeof(TipoRegistro));
//   Area->n = 0;  // numero de itens inicializado com 0
//   C->comp = 0;
//   C->transf = 0;
//   C->transfEscrita = 0;
//   C->transfLeitura = 0;

//   clock_t inicio;
//   clock_t fim;
// }

// void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq,
//                       int Dir, Contagem *C) {
//   int i, j;
//   TipoArea Area;
//   QuickInicializaArea(&Area, &C);
//   if (Dir - Esq < 1) return;
//   QuickParticao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j, Area);
//   if (i - Esq < Dir - j) {
//     QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, C);
//     QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, C);
//   } else {
//     QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, C);
//     QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, C);
//   }
// }
// void QuickLeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short
// *OndeLer) {
//   fseek(*ArqLEs, (*Ls - 1) * sizeof(TipoRegistro), SEEK_SET);
//   fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs);
//   (*Ls)--;
//   *OndeLer = false;
// }

// void QuickLeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer)
// {
//   fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi);
//   (*Li)++;
//   *OndeLer = true;
// }

// void QuickInserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea) {
//   QuickInsereItem(*UltLido, Area);
//   (*NRArea) = ObterNumeroCelulas(Area);
// }

// void QuickEscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es) {
//   // Escreve o maior elemento da área no final do arquivo de saída
//   fseek(*ArqLEs, (*Es - 1) * sizeof(TipoRegistro), SEEK_SET);
//   fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
//   (*Es)--;
// }

// void QuickEscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei) {
//   // Escreve o menor elemento da área no início do arquivo de saída
//   // fseek(*ArqEi, (*Ei - 1) * sizeof(TipoRegistro), SEEK_SET);
//   fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi);
//   (*Ei)++;
// }

// void QuickRetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea) {
//   QuickRetiraUltimo(Area, R);
//   (*NRArea) = QuickObterNumeroCelulas(Area);
// }

// void QuickRetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea) {
//   QuickRetiraPrimeiro(Area, R);
//   (*NRArea) = QuickObterNumeroCelulas(Area);
// }

// void QuickParticao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area,
//                    int Esq, int Dir, int *i, int *j, TipoArea area) {
//   int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq;
//   int Linf = INT_MIN, Lsup = INT_MAX;
//   // int NRArea = 0; // Número de elementos na área
//   short OndeLer = true;
//   TipoRegistro UltLido, R;
//   fseek(*ArqLi, (Li - 1) * sizeof(TipoRegistro), SEEK_SET);
//   fseek(*ArqLEs, (Es - 1) * sizeof(TipoRegistro), SEEK_SET);
//   *i = Esq - 1;
//   *j = Dir + 1;
//   while (Ls >= Li) {
//     if (area.n < TAMANHOAREA - 1) {  // verificar se tem parte do pivo vazio
//       if (OndeLer) {
//         QuickLeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
//       } else {
//         QuickLeInf(ArqLi, &UltLido, &Li, &OndeLer);
//         QuickInserirArea(&Area, &UltLido, &area);
//         continue;
//       }
//     }

//     if (Ls == Es)
//       // tem que fazer o numero de comparações
//       QuickLeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
//     else if (Li == Ei)
//       QuickLeInf(ArqLi, &UltLido, &Li, &OndeLer);
//     else if (OndeLer)
//       QuickLeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
//     else
//       QuickLeInf(ArqLi, &UltLido, &Li, &OndeLer);
//     if (UltLido.chave > Lsup) {
//       *j = Es;
//       QuickEscreveMax(ArqLEs, UltLido, &Es);
//       continue;
//     }

//     if (UltLido.chave < Linf) {
//       *i = Ei;
//       QuickEscreveMin(ArqEi, UltLido, &Ei);
//       continue;
//     }
//     QuickInserirArea(&Area, &UltLido, &area);
//     if (Ei - Esq < Dir - Es) {
//       QuickRetiraMin(&Area, &R, &area);
//       QuickEscreveMin(ArqEi, R, &Ei);
//       Linf = R.chave;
//     } else {
//       QuickRetiraMax(&Area, &R, &area);
//       QuickEscreveMax(ArqLEs, R, &Es);
//       Lsup = R.chave;
//     }
//   }

//   while (Ei <= Es) {
//     QuickRetiraMin(&Area, &R, &area);
//     QuickEscreveMin(ArqEi, R, &Ei);
//   }
// }