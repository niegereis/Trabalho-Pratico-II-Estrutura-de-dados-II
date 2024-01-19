#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "quickSortExterno.h"


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


void LeSup(FILE **arqLEs, Aluno *ultLido, int *ls, short *ondeLer){
    fseek(*arqLEs, (*ls-1) * ALUNO_LINHA, SEEK_SET);
    *ultLido = AlunoLer(*arqLEs);
    (*ls)--;
    *ondeLer = false;
}

void LeInf(FILE **arqLEi, Aluno *ultLido, int *li, short *ondeLer){
    fseek(*arqLEi, (*li - 1) * ALUNO_LINHA, SEEK_SET);
    *ultLido = AlunoLer(*arqLEi);
    (*li)++;
    *ondeLer = true;
}

void InsereItem(Aluno aluno, TipoArea *area){ // insere o ultimo lido na area
    int i = area->n;
    while (i > 0 && aluno.nota < area->r[i - 1].nota){
        area->r[i] = area->r[i - 1];
        i--;
    }
    area->r[i] = aluno;
    area->n++;
}
void inserirArea(TipoArea *area, Aluno *ultLido){
    InsereItem(*ultLido, area);
}


void EscreveMax(FILE **arqLEs, Aluno R, int *Es){ // Escreve o maior elemento da área no final do arquivo de saída
    // Escreve o maior elemento da área no final do arquivo de saída
    fseek(*arqLEs, (*Es - 1) * ALUNO_LINHA, SEEK_SET);
    alunoEscreve(*arqLEs, R);
    (*Es)--;
}

void EscreveMin(FILE **arqLEi, Aluno R, int *Ei){ // Escreve o menor elemento da área no início do arquivo de saída
    // Escreve o menor elemento da área no início do arquivo de saída
    fseek(*arqLEi, (*Ei - 1) * ALUNO_LINHA, SEEK_SET);
    alunoEscreve(*arqLEi, R);
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

void Particao(FILE **arqEi, FILE **arqLEs, TipoArea area, int Esq, int Dir, int *i, int *j, Analise *analise){
    int ls = Dir, Es = Dir, li = Esq, Ei = Esq; 
    float Linf = INT_MIN, Lsup = INT_MAX;
    short ondeLer = true;
    Aluno ultLido, R;
   
    *i = Esq - 1;
    *j = Dir + 1;

    analise->comparacoes++;
    while (ls >= li){
        analise->comparacoes++;
        if (area.n < TAMANHOAREA - 1){ // verificar se tem parte do pivo vazio
            if (ondeLer){
                analise->transferenciasLeitura++;
                LeSup(arqLEs, &ultLido, &ls, &ondeLer);
            }
            else{
                analise->transferenciasLeitura++;
                LeInf(arqEi, &ultLido, &li, &ondeLer);
            }
            inserirArea(&area, &ultLido);
            continue;
            
        }
        analise->comparacoes++;
        if (ls == Es){ // se o ls for igual ao Es, então o ls é o maior elemento da área
            analise->transferenciasLeitura++;
            LeSup(arqLEs, &ultLido, &ls, &ondeLer);
        }
        else if (li == Ei){
            analise->comparacoes++;
            analise->transferenciasLeitura++;
            LeInf(arqEi, &ultLido, &li, &ondeLer);
        }
        else if (ondeLer){
            analise->comparacoes += 2;
            analise->transferenciasLeitura++;
            LeSup(arqLEs, &ultLido, &ls, &ondeLer);
        }
        else {
            analise->comparacoes += 2;
            analise->transferenciasLeitura++;
            LeInf(arqEi, &ultLido, &li, &ondeLer);
        }

        
        analise->comparacoes++;
        if (ultLido.nota > Lsup){ // se o ultimo lido for maior que o limite superior
            analise->transferenciasEscrita++;
            *j = Es;
            EscreveMax(arqLEs, ultLido, &Es);
            continue;
        }

        analise->comparacoes++;
        if(ultLido.nota < Linf){
            analise->transferenciasEscrita++;
            *i = Ei;
            EscreveMin(arqEi, ultLido, &Ei);
            continue;
        }

        // colocar a funçao de inserir pivo
        // ------------------------------------------------------

        inserirArea(&area, &ultLido);

        // ------------------------------------------------------
        analise->comparacoes++;
        if (Ei - Esq < Dir - Es){ // retira o aluno com maior nota
        //retirar o maior item do pivo
            analise->transferenciasEscrita++;
            RetiraMin(&area, &R);
            EscreveMin(arqEi, R, &Ei);
            Linf = R.nota;
        } else { // retira o aluno com menor nota
        //retira menor item do pivo
            analise->transferenciasEscrita++;
            RetiraMax(&area, &R);
            EscreveMax(arqLEs, R, &Es);
            Lsup = R.nota;
        }
    }

    analise->comparacoes++;
    while (Ei <= Es){
        analise->comparacoes++; // numero de comparações
        analise->transferenciasEscrita++; // numero de transferencias de escrita
        RetiraMin(&area, &R);
        EscreveMin(arqEi, R, &Ei);
    }
}

void QuickSortExterno(FILE **arqLEi, FILE **arqLEs, int Esq, int Dir, Analise *analise){ // função recusiva
    int i, j;
    TipoArea area;
    area.n =0;
    if (Dir - Esq < 1) return;
    Particao(arqLEi, arqLEs, area, Esq, Dir, &i, &j, analise);
    

    if (i - Esq < Dir - j){
        QuickSortExterno(arqLEi, arqLEs, Esq, i, analise);
        QuickSortExterno(arqLEi, arqLEs, j, Dir, analise);
    } else {
        QuickSortExterno(arqLEi, arqLEs, j, Dir, analise);
        QuickSortExterno(arqLEi, arqLEs, Esq, i, analise);
    }

    
}

void OrdenarQS(Analise *analise, int quantidade) {
  struct timespec inicio, fim;
    FILE *arqLEi, *arqLEs;
    //arqLi = fopen("output.txt","r+");
    arqLEi = fopen("./arquivos/output.txt","r+");
    arqLEs = fopen("./arquivos/output.txt","r+");
    if(arqLEi == NULL || arqLEs == NULL){
        perror("error: ");
        exit(1);
    }
    
  clock_gettime(CLOCK_REALTIME, &inicio);

  QuickSortExterno(&arqLEi, &arqLEs, 1, quantidade, analise);

  clock_gettime(CLOCK_REALTIME, &fim);

  AnaliseDefinirTempoPeloInicioEFim(analise, inicio, fim);
    //fclose(arqLi);
    fclose(arqLEi);
    fclose(arqLEs);
}

