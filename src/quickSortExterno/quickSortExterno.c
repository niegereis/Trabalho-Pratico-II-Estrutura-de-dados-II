#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "quickSortExterno.h"
#include "aluno.h"
#include "compartilhado.h"
#define TAMANHOAREA 3

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

void imprimeArea(TipoArea a){ /// terminar isso aq depois
    int i;
    printf("\n");
    for(i = 0; i < TAMANHOAREA; i++){
        if(i < a.n)
            printf("|%.1f ", a.r[i].nota);
        else    
            printf("|   ");
    }
    printf("|\n");
}

void inicializaArea(TipoArea *area){
    area->r = (Aluno*)malloc(TAMANHOAREA * sizeof(Aluno));
    area->n = 0;

}
//ordenando area do quicksort externo
// void inserirEOrdenarArea(TipoArea *area, Aluno aluno, Contagem *C){
//     int i = area->n;
//     while (i > 0 && aluno.nota < area->r[i - 1].nota){
//         area->r[i] = area->r[i - 1];
//         i--;
//         C->comp++;
//     }
//     area->r[i] = aluno;
//     area->n++;

// }

int ObterNumeroCelulas(TipoArea area){return area.n;}



void LeSup(FILE **arqLEs, Aluno *ultLido, int *ls, short *ondeLer){
    //fseek(*arqLEs, (*ls - 1) * sizeof(Aluno), SEEK_SET);
    fseek(*arqLEs, (*ls-1) * ALUNO_LINHA, SEEK_SET);
    //fread(ultLido, sizeof(Aluno), 1, *arqLEs);
    *ultLido = AlunoLer(*arqLEs);
    //fseek(*arqLEs, (*ls) * ALUNO_LINHA, SEEK_SET);
    (*ls)--;
    *ondeLer = false;
}

void LeInf(FILE **arqLi, Aluno *ultLido, int *li, short *ondeLer){
    //fread(ultLido, sizeof(Aluno), 1, *arqLi);
    fseek(*arqLi, (*li - 1) * ALUNO_LINHA, SEEK_SET);
    *ultLido = AlunoLer(*arqLi);
    //fseek(*arqLi, (*li) * ALUNO_LINHA, SEEK_SET);
    (*li)++;
    *ondeLer = true;
}

void InsereItem(Aluno aluno, TipoArea *area){ // insere o ultimo lido na area
    int i = area->n;
    while (i > 0 && aluno.nota < area->r[i - 1].nota){
        area->r[i] = area->r[i - 1];
        i--;
        //C->comp++;
    }
    area->r[i] = aluno;
    printf("\narea n: %d\n", area->n);
    printf("\nnota: %.1f\n", aluno.nota);
    area->n++;
    printf("\narea n: %d...\n", area->n);
}
void inserirArea(TipoArea *area, Aluno *ultLido){
    InsereItem(*ultLido, area);
}


void EscreveMax(FILE **arqLEs, Aluno R, int *Es){ // Escreve o maior elemento da área no final do arquivo de saída
    // Escreve o maior elemento da área no final do arquivo de saída
    //fseek(*arqLEs, (*Es - 1) * sizeof(Aluno), SEEK_SET);
    fseek(*arqLEs, (*Es - 1) * ALUNO_LINHA, SEEK_SET);
    //fwrite(&R, sizeof(Aluno), 1, *arqLEs);
    alunoEscreve(*arqLEs, R);

    fseek(*arqLEs, (*Es) * ALUNO_LINHA, SEEK_SET);
    
    (*Es)--;
}

void EscreveMin(FILE **arqEi, Aluno R, int *Ei){ // Escreve o menor elemento da área no início do arquivo de saída
    // Escreve o menor elemento da área no início do arquivo de saída
    //fseek(*arqEi, (*Ei - 1) * sizeof(Aluno), SEEK_SET);
    fseek(*arqEi, (*Ei - 1) * ALUNO_LINHA, SEEK_SET);

    //fwrite(&R, sizeof(Aluno), 1, *arqEi);
    alunoEscreve(*arqEi, R);
    fseek(*arqEi, (*Ei) * ALUNO_LINHA, SEEK_SET);
    (*Ei)++;
}

// ---------------------------- RETIRANDO O ULTIMO ITEM DA AREA ----------------------------

void RetiraMax(TipoArea *area, Aluno *aluno){
    *aluno = area->r[area->n - 1];
    area->n--;
}

// -----------------------------------------------------------------------------------------

// ---------------------------- RETIRANDO O PRIMEIRO ITEM DA AREA ----------------------------

void RetiraMin(TipoArea *area, Aluno *aluno){
    *aluno = area->r[0];
    area->n--;
    for (int i = 0; i < area->n; i++){
        area->r[i] = area->r[i + 1];
    }
}

// ------------------------------------------------------------------------------------------   

void Particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, TipoArea area, int Esq, int Dir, int *i, int *j, Contagem *C){
    int ls = Dir, Es = Dir, li = Esq, Ei = Esq; 
    float Linf = INT_MIN, Lsup = INT_MAX;
    //int NRArea = 0; // Número de elementos na área
    short ondeLer = true;
    Aluno ultLido, R;
   
   // fseek(*arqLi, (li - 1) * ALUNO_LINHA, SEEK_SET); //   Escreve o menor elemento da área no início do arquivo de saída
   // fseek(*arqEi, (Ei - 1) * ALUNO_LINHA, SEEK_SET);
    //fseek(*arqLEs, (Es - 1) * ALUNO_LINHA, SEEK_SET); // Escreve o maior elemento da área no final do arquivo de saída
    *i = Esq - 1;
    *j = Dir + 1;
    while (ls >= li){
        if (area.n < TAMANHOAREA - 1){ // verificar se tem parte do pivo vazio
            if (ondeLer){
                printf("\n%d ->",ls);
                LeSup(arqLEs, &ultLido, &ls, &ondeLer);
                printf("Lido sup: %.1f", ultLido.nota);
            }
            else{
                printf("\n%d ->",li);
                LeInf(arqLi, &ultLido, &li, &ondeLer);
                printf("Lido inf: %.1f", ultLido.nota);
            }
            inserirArea(&area, &ultLido);
            printf("Entrou1\n");
            imprimeArea(area);
            continue;
            
        }

        if (ls == Es){ // se o ls for igual ao Es, então o ls é o maior elemento da área
            printf("\n%d ->",ls);
            LeSup(arqLEs, &ultLido, &ls, &ondeLer);
            printf("Lido sup: %.1f", ultLido.nota);
        }
        else if (li == Ei){
            printf("\n%d ->",li);
            LeInf(arqLi, &ultLido, &li, &ondeLer);
            printf("Lido inf: %.1f", ultLido.nota);
        }
        else if (ondeLer){
            printf("\n%d ->",ls);
            LeSup(arqLEs, &ultLido, &ls, &ondeLer);
            printf("Lido sup: %.1f", ultLido.nota);
        }
        else {
            printf("\n%d ->",li);
            LeInf(arqLi, &ultLido, &li, &ondeLer);
            printf("Lido inf: %.1f", ultLido.nota);
        }

        
        if (ultLido.nota > Lsup){ // se o ultimo lido for maior que o limite superior
            C->comp++;
            *j = Es;
            printf("Maior que lim %.1f -> %d\n", ultLido.nota, Es);
            EscreveMax(arqLEs, ultLido, &Es);
            continue;
        }

        if(ultLido.nota < Linf){
            C->comp++;
            *i = Ei;
            printf("Menor que lim %.1f -> %d\n", ultLido.nota, Ei);
            EscreveMin(arqEi, ultLido, &Ei);
            continue;
        }

        // colocar a funçao de inserir pivo
        // ------------------------------------------------------

        inserirArea(&area, &ultLido);
        printf("Entrou %.1f\n", ultLido.nota);
        imprimeArea(area);

        // ------------------------------------------------------
        if (Ei - Esq < Dir - Es){ // retira o aluno com maior nota
        //retirar o maior item do pivo
            C->transfEscrita++;
            RetiraMin(&area, &R);
            printf("saiu inf %.1f -> %d\n", R.nota, Ei);
            EscreveMin(arqEi, R, &Ei);
            Linf = R.nota;
        } else { // retira o aluno com menor nota
        //retira menor item do pivo
            C->transfEscrita++;
            RetiraMax(&area, &R);
            printf("saiu sup %.1f -> %d\n", R.nota, Es);
            EscreveMax(arqLEs, R, &Es);
            Lsup = R.nota;
        }
    }
    printf("Quebra\n");

    while (Ei <= Es){
        C->transfEscrita++; // numero de transferencias de escrita
        C->comp++; // numero de comparações
        RetiraMin(&area, &R);
        printf("saiu inf %.1f -> %d\n", R.nota, Ei);
        EscreveMin(arqEi, R, &Ei);
    }
    printf("i:%d j:%d", *i, *j);
}

void QuickSortExterno(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int Esq, int Dir, Contagem *C){ // função recusiva
    int i, j;
    TipoArea area;
    inicializaArea(&area);
    if (Dir - Esq < 1) return;
    Particao(arqLi, arqEi, arqLEs, area, Esq, Dir, &i, &j, C);
    printf("i:%d j:%d", i, j);
    if (i - Esq < Dir - j){
        QuickSortExterno(arqLi, arqEi, arqLEs, Esq, i, C);
        QuickSortExterno(arqLi, arqEi, arqLEs, j, Dir, C);
    } else {
        QuickSortExterno(arqLi, arqEi, arqLEs, j, Dir, C);
        QuickSortExterno(arqLi, arqEi, arqLEs, Esq, i, C);
    }

    
}

void OrdenarQS(Contagem *contagem, FILE *arquivo, int quantidade) {
  struct timespec inicio, fim;
    FILE *arqLi, *arqEi, *arqLEs;
    fseek(arquivo, 0, 0);
    arqLi = fopen("output.txt","r+");
    arqEi = fopen("output.txt","r+");
    arqLEs = fopen("output.txt","r+");
    
  clock_gettime(CLOCK_REALTIME, &inicio);

  QuickSortExterno(&arqLi, &arqEi, &arqLEs, 1, quantidade, contagem);

  clock_gettime(CLOCK_REALTIME, &fim);

  contagem->tempo = (fim.tv_sec - inicio.tv_sec) * 1e9 + (fim.tv_nsec - inicio.tv_nsec);

    fclose(arqLi);
    fclose(arqEi);
    fclose(arqLEs);
}

