#include <stdio.h>
#include <stdlib.h>

typedef int TipoChave;
typedef struct TipoItem {
    TipoChave Chave;
    /* outros componentes */
} TipoItem;
typedef struct TipoArea {
    TipoItem *Itens;
    int Max;
    int NumCelOcupadas;
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

// typedef de tipoArea
