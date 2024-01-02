#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef long TipoChave;

typedef struct TipoRegistro {
    TipoChave chave;
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;

// struct de tipoArea

typedef struct TipoArea {
    TipoRegistro *r;
    int n;
} TipoArea;

void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir){
    int i, j;
    TipoArea Area;
    FAVazia(&Area);
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
    int NRArea = 0; // Número de elementos na área
    short OndeLer = true;
    TipoRegistro UltLido, R;
    *i = Esq - 1;
    *j = Dir + 1;
    while (Li <= Ls){
        if (ObterNumeroCelulas(&Area) == 0){
            if (OndeLer){
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            } else {
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            }
            inserirArea(&Area, &UltLido, &NRArea);
        }
        if (OndeLer){
            RetiraMax(&Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            if (R.chave < UltLido.chave){
                UltLido = R;
                *j = Es;
                *i = Ei;
            }
        } else {
            RetiraMin(&Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            if (R.chave > UltLido.chave){
                UltLido = R;
                *i = Ei;
                *j = Es;
            }
        }
    }
    while (Ei <= Es){
        if (OndeLer){
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        } else {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }
        if (UltLido.chave < R.chave){
            EscreveMax(ArqLEs, R, &Es);
            EscreveMin(ArqEi, UltLido, &Ei);
        } else {
            EscreveMax(ArqEi, UltLido, &Ei);
            EscreveMin(ArqLEs, R, &Es);
        }
    }