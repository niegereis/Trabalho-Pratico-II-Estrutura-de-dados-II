#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "quickSortExterno.h"
#include "../aluno/aluno.h"
#include "../compartilhado/compartilhado.h"
#include "../heap/heap.h"
#include "../aluno/aluno.h"
#define TAMANHOAREA 10

//Para o método de quickSort externo, deve ser considerada a existência de memória interna disponível para armazenar um vetor
// de, no máximo, 10 registros
//A 2ª fase deste trabalho corresponde à análise experimental da complexidade de desempenho dos 3
/*métodos mencionados, considerando os seguintes quesitos:
1) número de transferências (leitura) de registros da memória externa para a memória interna;
2) número de transferências (escrita) de registros da memória interna para a memória externa;
3) número de comparações entre valores do campo de ordenação dos registros;
4) tempo de execução (tempo do término de execução menos o tempo do início de execução).*/


// Para a 2ª fase, experimentos devem ser realizados considerando o arquivo texto "PROVAO.TXT"
/*arquivo disponível na página da disciplina) que corresponde a uma base de dados de notas de 471.705
alunos do Ensino Superior que fizeram o "Provão" em 2003 (último ano em que tal avaliação foi
aplicada). Cada linha do arquivo contém os seguintes dados relativos a um determinado aluno que fez o
"Provão":
- número de inscrição do aluno (valor inteiro longo, ocupando as colunas 1 a 8 do arquivo texto);
- nota obtida pelo aluno (valor real entre 0.0 e 100.0, ocupando as colunas 10 a 14 do arquivo texto);
- estado do aluno (cadeia de 2 caracteres, ocupando as colunas 16 e 17 do arquivo texto);
- cidade do aluno (cadeia de 50 caracteres, ocupando as colunas 19 a 68 do arquivo texto);
- curso do aluno (cadeia de 30 caracteres, ocupando as colunas 70 a 99 do arquivo texto).*/

void leArquivoEscreveArquivo(Aluno *aluno, FILE* arquivo, int quantidade){ /// terminar isso aq depois
    int i;
    for (i = 0; i <quantidade; i++){
       *aluno = AlunoLer(arquivo);

    }
    fclose(arquivo);
}

void inicializaArea(TipoArea *Area, Contagem *C){
    Area->r = (Aluno*)malloc(TAMANHOAREA * sizeof(Aluno));
    Area->n = 0;
    C->comp = 0;
    C->transfEscrita = 0;
    C->transfLeitura = 0;


}
//ordenando area do quicksort externo
void inserirEOrdenarArea(TipoArea *Area, Aluno aluno, Contagem *C){
    int i = Area->n;
    while (i > 0 && aluno.nota < Area->r[i - 1].nota){
        Area->r[i] = Area->r[i - 1];
        i--;
        C->comp++;
    }
    Area->r[i] = aluno;
    Area->n++;

}



void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir, Contagem *C){ // função recusiva
    int i, j;
    TipoArea Area;
    inicializaArea(&Area, &C);
    if (Dir - Esq < 1) return;
    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j, Area, C);
    if (i - Esq < Dir - j){
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, C);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, C);
    } else {
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, C);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, C);
    }

    
}
void LeSup(FILE **ArqLEs, Aluno *UltLido, int *Ls, short *OndeLer){
    fseek(*ArqLEs, (*Ls - 1) * sizeof(Aluno), SEEK_SET);
    fread(UltLido, sizeof(Aluno), 1, *ArqLEs);
    (*Ls)--;
    *OndeLer = false;
}

void LeInf(FILE **ArqLi, Aluno *UltLido, int *Li, short *OndeLer){
    fread(UltLido, sizeof(Aluno), 1, *ArqLi);
    (*Li)++;
    *OndeLer = true;
}

void inserirArea(TipoArea *Area, Aluno *UltLido, int *NRArea){
    InsereItem(*UltLido, Area);
    (*NRArea) = ObterNumeroCelulas(Area);
}

void InsereItem(Aluno UltLido, TipoArea *Area){ // insere o ultimo lido na area
    Area->r[Area->n] = UltLido;
    Area->n++;
}

void EscreveMax(FILE **ArqLEs, Aluno R, int *Es){ // Escreve o maior elemento da área no final do arquivo de saída
    // Escreve o maior elemento da área no final do arquivo de saída
    fseek(*ArqLEs, (*Es - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R, sizeof(Aluno), 1, *ArqLEs);
    (*Es)--;
}

void EscreveMin(FILE **ArqEi, Aluno R, int *Ei){ // Escreve o menor elemento da área no início do arquivo de saída
    // Escreve o menor elemento da área no início do arquivo de saída
    fseek(*ArqEi, (*Ei - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R, sizeof(Aluno), 1, *ArqEi);
    (*Ei)++;
}

// ---------------------------- RETIRANDO O ULTIMO ITEM DA AREA ----------------------------

void RetiraMax(TipoArea *Area, Aluno *aluno, int *NRArea){
    RetiraUltimo(Area, aluno);
    (*NRArea) = ObterNumeroCelulas(Area);
}
void RetiraUltimo(TipoArea *Area, Aluno *aluno){
    *aluno = Area->r[Area->n - 1];
    Area->n--;
}

// -----------------------------------------------------------------------------------------

// ---------------------------- RETIRANDO O PRIMEIRO ITEM DA AREA ----------------------------
void RetiraMin(TipoArea *Area, Aluno *aluno, int *NRArea){
    RetiraPrimeiro(Area, aluno);
    (*NRArea) = ObterNumeroCelulas(Area);
}

void RetiraPrimeiro(TipoArea *Area, Aluno *aluno){
    *aluno = Area->r[0];
    Area->n--;
    for (int i = 0; i < Area->n; i++){
        Area->r[i] = Area->r[i + 1];
    }
}
// ------------------------------------------------------------------------------------------   

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j, TipoArea area, Contagem *C){
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq; 
    int Linf = INT_MIN, Lsup = INT_MAX;
    //int NRArea = 0; // Número de elementos na área
    short OndeLer = true;
    Aluno UltLido, R;
    fseek(*ArqLi, (Li - 1) * sizeof(Aluno), SEEK_SET); //   Escreve o menor elemento da área no início do arquivo de saída
    fseek(*ArqLEs, (Es - 1) * sizeof(Aluno), SEEK_SET); // Escreve o maior elemento da área no final do arquivo de saída
    *i = Esq - 1;
    *j = Dir + 1;
    while (Ls >= Li){
        if (area.n < TAMANHOAREA - 1){ // verificar se tem parte do pivo vazio
            if (OndeLer){
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            } else {
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
                inserirArea(&Area, &UltLido, &area);
                continue;
            }
        }

        if (Ls == Es){ // se o Ls for igual ao Es, então o Ls é o maior elemento da área
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        }
        else if (Li == Ei){
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }
        else if (OndeLer){
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        }
        else {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }

        
        if (UltLido.nota > Lsup){ // se o ultimo lido for maior que o limite superior
            C->comp++;
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }

        if(UltLido.nota < Linf){
            C->comp++;
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }

        // colocar a funçao de inserir pivo
        // ------------------------------------------------------

        inserirArea(&Area, &UltLido, &area);

        // ------------------------------------------------------
        if (Ei - Esq < Dir - Es){ // retira o aluno com maior nota
        //retirar o maior item do pivo
            C->transfEscrita++;
            RetiraMin(&Area, &R, &area);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.nota;
        } else { // retira o aluno com menor nota
        //retira menor item do pivo
            C->transfEscrita++;
            RetiraMax(&Area, &R, &area);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.nota;
        }
    }

    while (Ei <= Es){
        C->transfEscrita++; // numero de transferencias de escrita
        C->comp++; // numero de comparações
        RetiraMin(&Area, &R, &area);
        EscreveMin(ArqEi, R, &Ei);
    }
}

void OrdenarQS(Contagem *contagem, FILE *arquivo, int quantidade) {
  struct timespec inicio, fim;
    FILE *arqLi, *arqEi, *arqLEs;
    fseek(arquivo, 0, 0);
    arqLi = arquivo;
    arqEi = arquivo;
    fseek(arquivo, (quantidade-1)*100, 0);
    arqLEs = arquivo;
    
  clock_gettime(CLOCK_REALTIME, &inicio);

  QuickSortExterno(&arqLi, &arqEi, &arqLEs, 0, quantidade-1, contagem);

  clock_gettime(CLOCK_REALTIME, &fim);

  contagem->tempo = (fim.tv_sec - inicio.tv_sec) * 1e9 + (fim.tv_nsec - inicio.tv_nsec);

    fclose(arqLi);
    fclose(arqEi);
    fclose(arqLEs);
}