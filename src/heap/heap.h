

#ifndef HEAP_H
#define HEAP_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../aluno/aluno.h"
#include "../compartilhado/compartilhado.h"
typedef struct itemHeap {
  Aluno aluno;
  int fitaDeOrigem;
  bool marcado;
} ItemHeap;

typedef enum tipoHeap { HEAP_MAX, HEAP_MIN } TipoHeap;

typedef struct heap {
  ItemHeap *itensHeap;
  ItemHeap *ultimoRemovido;
  TipoHeap tipo;
  int qtdItens;
  int qtdItensMarcados;
  int qtdMaxima;
} Heap;

Heap HeapCriar(int tam, TipoHeap tipo);
bool HeapInserir(Heap *heap, Aluno *aluno, Analise *analise);
bool HeapInserirComFitaOrigem(Heap *heap, Aluno *aluno, int fitaDeOrigem, Analise *analise);
void HeapImprime(Heap *heap);
bool HeapRemove(Heap *heap, ItemHeap *itemRemovido, Analise *analise);
void ItemHeapImprime(ItemHeap *itemHeap);
void HeapDesmarcarTodosEReconstituir(Heap *heap, Analise *analise);
void HeapEsvaziar(Heap *h, Analise *analise);
void HeapImprimeValidade(Heap *h, Analise *analise);
void HeapRemoverDesmarcados(Heap *heap);
bool HeapCheio(Heap *heap);

bool HeapItemATemMesmoPesoOuMaiorQueB(Heap *h, ItemHeap *a, ItemHeap *b, Analise *analise);
bool HeapItemAPesoMaiorQueB(Heap *h, ItemHeap *a, ItemHeap *b, Analise *analise);
int HeapObterPosicaoPrimeiroItemMarcado(Heap *heap);

bool HeapVerificaSeEhValido(Heap *heap, Analise *analise);
bool HeapVerificaSeEhValidoRecursivo(Heap *heap, int centro, bool valido, Analise *analise);

#endif // HEAP_H