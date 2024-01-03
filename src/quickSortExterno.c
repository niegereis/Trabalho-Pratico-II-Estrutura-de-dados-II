#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../headers/quickSortExterno.h"
#define TAMANHOAREA 10
//Para o método de quickSort externo, deve ser considerada a existência de memória interna disponível para armazenar um vetor
// de, no máximo, 10 registros
//A 2ª fase deste trabalho corresponde à análise experimental da complexidade de desempenho dos 3
/*métodos mencionados, considerando os seguintes quesitos:
1) número de transferências (leitura) de registros da memória externa para a memória interna;
2) número de transferências (escrita) de registros da memória interna para a memória externa;
3) número de comparações entre valores do campo de ordenação dos registros;
4) tempo de execução (tempo do término de execução menos o tempo do início de execução).*/


//A area seria o pivo
void inicializaArea(TipoArea *Area){
    Area->r = (TipoRegistro*) malloc(10 * sizeof(TipoRegistro));
    Area->n = 0; // numero de itens inicializado com 0
}



void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir){
    int i, j;
    TipoArea Area;
    inicializaArea(&Area);
    if (Dir - Esq < 1) return;
    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j);
    if (i - Esq < Dir - j){
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
    } else {
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
    }

    
}
void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer){
    fseek(*ArqLEs, (*Ls - 1) * sizeof(TipoRegistro), SEEK_SET);
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Ls)--;
    *OndeLer = false;
}

void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer){
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi);
    (*Li)++;
    *OndeLer = true;
}

void inserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea){
    InsereItem(*UltLido, Area);
    (*NRArea) = ObterNumeroCelulas(Area);
}

void EscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es){
    // Escreve o maior elemento da área no final do arquivo de saída
    fseek(*ArqLEs, (*Es - 1) * sizeof(TipoRegistro), SEEK_SET);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Es)--;
}

void EscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei){
    // Escreve o menor elemento da área no início do arquivo de saída
    //fseek(*ArqEi, (*Ei - 1) * sizeof(TipoRegistro), SEEK_SET);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi);
    (*Ei)++;
}

void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea){
    RetiraUltimo(Area, R);
    (*NRArea) = ObterNumeroCelulas(Area);
}

void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea){
    RetiraPrimeiro(Area, R);
    (*NRArea) = ObterNumeroCelulas(Area);
}


void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j){
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq; 
    int Linf = INT_MIN, Lsup = INT_MAX;
    int NRArea = 0; // Número de elementos na área
    short OndeLer = true;
    TipoRegistro UltLido, R;
    fseek(*ArqLi, (Li - 1) * sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqLEs, (Es - 1) * sizeof(TipoRegistro), SEEK_SET);
    *i = Esq - 1;
    *j = Dir + 1;
    while (Ls >= Li){
        if (NRArea < TAMANHOAREA - 1){
            if (OndeLer){
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            } else {
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
                inserirArea(&Area, &UltLido, &NRArea);
                continue;
            }
        }

        if (Ls == Es)
        LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else if (Li == Ei) LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            else if (OndeLer) LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
                else LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        if (UltLido.chave > Lsup){
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }

        if(UltLido.chave < Linf){
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }
        inserirArea(&Area, &UltLido, &NRArea);
        if (Ei - Esq < Dir - Es){
            RetiraMin(&Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.chave;
        } else {
            RetiraMax(&Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.chave;
        }
    }

    while (Ei <= Es){
        RetiraMin(&Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
    }
    


}