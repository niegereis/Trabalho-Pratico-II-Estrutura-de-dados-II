

#ifndef HEAP_H
#define HEAP_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../aluno/aluno.h"

typedef struct itemHeap {
  Aluno aluno;
  bool marcado;
} ItemHeap;

typedef enum tipoHeap { HEAP_MAX, HEAP_MIN } TipoHeap;

typedef struct heap {
  ItemHeap *itensHeap;
  ItemHeap ultimoRemovido;
  TipoHeap tipo;
  int qtdItens;
  int qtdItensMarcados;
  int qtdMaxima;
} Heap;

Heap HeapCriar(int tam, TipoHeap tipo);
bool HeapInserir(Heap *heap, Aluno aluno, bool marcado);
void HeapImprime(Heap *heap);
bool HeapRemove(Heap *heap, ItemHeap *alunoObtido);
void ItemHeapImprime(ItemHeap *itemHeap);
void HeapDesmarcarTodosEReconstituir(Heap *heap);
bool HeapVerificaSeEhValido(Heap *heap);
void HeapEsvaziar(Heap *h);
void HeapImprimeValidade(Heap *h);

bool HeapVerificaSeEhValidoRecursivo(Heap *heap, int centro, bool valido);
bool HeapItemATemMesmoPesoOuMaiorQueB(Heap *h, ItemHeap *a, ItemHeap *b);
bool HeapItemAPesoMaiorQueB(Heap *h, ItemHeap *a, ItemHeap *b);
int HeapObterPosicaoPrimeiroItemMarcado(Heap *heap);

#endif // HEAP_H