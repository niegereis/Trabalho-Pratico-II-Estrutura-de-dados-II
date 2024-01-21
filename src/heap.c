#include "heap.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>


void HeapTroca(Heap *heap, int p1, int p2);
void HeapReconstituir(Heap *heap, int pos, Analise *analise);

ItemHeap ItemHeapCriar(Aluno *aluno, int fitaDeOrigem) {
  ItemHeap itemHeap;
  itemHeap.aluno = *aluno;
  itemHeap.fitaDeOrigem = fitaDeOrigem;
  itemHeap.marcado = false;
  return itemHeap;
}

float ItemHeapObterNota(ItemHeap *itemHeap) { return itemHeap->aluno.nota; }

Heap HeapCriar(int tam, TipoHeap tipo) {
  Heap heap;
  heap.qtdItens = 0;
  heap.itensHeap = malloc(sizeof(ItemHeap) * tam);
  heap.qtdMaxima = tam;
  heap.qtdItensMarcados = 0;
  heap.ultimoRemovido = NULL;
  heap.tipo = tipo;
  return heap;
}

bool HeapInserir(Heap *heap, Aluno *aluno, Analise *analise) {
  if ((heap->qtdItens + heap->qtdItensMarcados) == heap->qtdMaxima)
    return false;

  ItemHeap novoItem = ItemHeapCriar(aluno, -1);

  bool ehParaInserirMarcado =
      heap->ultimoRemovido != NULL && HeapItemAPesoMaiorQueB(heap, &novoItem, heap->ultimoRemovido, analise);

  if (ehParaInserirMarcado) {
    novoItem.marcado = true;

    int posicaoNovoItemMarcado = heap->qtdMaxima - heap->qtdItensMarcados - 1;
    heap->itensHeap[posicaoNovoItemMarcado] = novoItem;
    heap->qtdItensMarcados++;
  } else {
    heap->itensHeap[heap->qtdItens++] = novoItem;
    for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
      HeapReconstituir(heap, i, analise);
  }

  return true;
}

bool HeapInserirComFitaOrigem(Heap *heap, Aluno *aluno, int fitaDeOrigem, Analise *analise) {
  if (HeapCheio(heap))
    return false;

  ItemHeap novoItem = ItemHeapCriar(aluno, fitaDeOrigem);
  heap->itensHeap[heap->qtdItens++] = novoItem;
  for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
    HeapReconstituir(heap, i, analise);

  return true;
}

bool HeapCheio(Heap *heap) { return heap->qtdItens + heap->qtdItensMarcados == heap->qtdMaxima; }

void HeapReconstituir(Heap *heap, int pos, Analise *analise) {
  if (pos < 0)
    return;

  int esq = 2 * pos + 1;
  int dir = 2 * pos + 2;
  if (dir >= heap->qtdItens)
    dir--;

  if (esq >= heap->qtdItens)
    return;

  ItemHeap itemHeapCentro = heap->itensHeap[pos];
  ItemHeap itemHeapEsquerda = heap->itensHeap[esq];
  ItemHeap itemHeapDireita = heap->itensHeap[dir];

  bool direitaPrioridadeSobreEsquerda = HeapItemAPesoMaiorQueB(heap, &itemHeapDireita, &itemHeapEsquerda, analise);
  bool direitaPrioridadeSobreCentro = HeapItemAPesoMaiorQueB(heap, &itemHeapDireita, &itemHeapCentro, analise);
  if (direitaPrioridadeSobreEsquerda && direitaPrioridadeSobreCentro) {
    HeapTroca(heap, pos, dir);
    HeapReconstituir(heap, dir, analise);
    return;
  }

  bool esquerdaMesmoPesoOuMaiorSobreDireita =
      HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapEsquerda, &itemHeapDireita, analise);
  bool esquerdaPrioridadeSobreCentro = HeapItemAPesoMaiorQueB(heap, &itemHeapEsquerda, &itemHeapCentro, analise);
  if (esquerdaMesmoPesoOuMaiorSobreDireita && esquerdaPrioridadeSobreCentro) {
    HeapTroca(heap, pos, esq);
    HeapReconstituir(heap, esq, analise);
  }
}

void HeapDesmarcarTodosEReconstituir(Heap *heap, Analise *analise) {
  int posicaoPrimeiroItemMarcado = HeapObterPosicaoPrimeiroItemMarcado(heap);

  for (int i = posicaoPrimeiroItemMarcado + 1; i < heap->qtdMaxima; i++) {
    heap->itensHeap[i].marcado = false;
    if (heap->qtdItens + heap->qtdItensMarcados < heap->qtdMaxima)
      heap->itensHeap[heap->qtdItens] = heap->itensHeap[i];
    heap->qtdItens++;
    heap->qtdItensMarcados--;
  }

  for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
    HeapReconstituir(heap, i, analise);
}

int HeapObterPosicaoPrimeiroItemMarcado(Heap *heap) { return heap->qtdMaxima - heap->qtdItensMarcados - 1; }
void HeapTroca(Heap *heap, int p1, int p2) {
  ItemHeap aux = heap->itensHeap[p1];
  heap->itensHeap[p1] = heap->itensHeap[p2];
  heap->itensHeap[p2] = aux;
}

bool HeapRemove(Heap *heap, ItemHeap *itemObtido, Analise *analise) {
  if (heap->qtdItens <= 0)
    return false;

  ItemHeap ultimoItem = heap->itensHeap[heap->qtdItens - 1];

  *itemObtido = heap->itensHeap[0];
  heap->ultimoRemovido = itemObtido;

  heap->qtdItens--;

  heap->itensHeap[0] = ultimoItem;

  for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
    HeapReconstituir(heap, i, analise);

  return true;
}

void ItemHeapImprime(ItemHeap *itemHeap) {
  //AlunoImprime(&itemHeap->aluno);
  printf(" (%s marcado!)", itemHeap->marcado ? "Está" : "Não está");
  printf("\n");
}

void HeapImprime(Heap *heap) {
  int posicaoPrimeiroItemMarcado = HeapObterPosicaoPrimeiroItemMarcado(heap);
  printf("\n");
  printf("ESTADO ATUAL HEAP (%d itens não marcados) (%d itens marcados)\n\n", heap->qtdItens, heap->qtdItensMarcados);
  for (int i = 0; i < heap->qtdMaxima; i++) {
    if (i < heap->qtdItens || i > posicaoPrimeiroItemMarcado)
      ItemHeapImprime(&heap->itensHeap[i]);
    else
      printf("... VAZIO ...\n");
  }
}

bool HeapVerificaSeEhValidoRecursivo(Heap *heap, int centro, bool valido, Analise *analise) {
  if (centro > floor(heap->qtdItens / 2) - 1)
    return true;

  int esquerda = 2 * centro;
  int direita = 2 * centro + 1;
  if (direita >= heap->qtdItens)
    direita--;

  ItemHeap itemHeapCentro = heap->itensHeap[centro];
  ItemHeap itemHeapEsquerda = heap->itensHeap[esquerda];
  ItemHeap itemHeapDireita = heap->itensHeap[direita];

  bool centroTemPrioridadeADireita = HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapCentro, &itemHeapDireita, analise);
  bool centroPrioridadeEsquerda = HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapCentro, &itemHeapEsquerda, analise);

  bool posicaoValida = centroTemPrioridadeADireita && centroPrioridadeEsquerda;

  bool esquerdaValida = true;
  if (centro != esquerda)
    esquerdaValida = HeapVerificaSeEhValidoRecursivo(heap, esquerda, valido && posicaoValida, analise);

  bool direitaValida = HeapVerificaSeEhValidoRecursivo(heap, direita, valido && posicaoValida, analise);
  return direitaValida && esquerdaValida;
}

bool HeapVerificaSeEhValido(Heap *heap, Analise *analise) { return HeapVerificaSeEhValidoRecursivo(heap, 0, true, analise); }

bool HeapItemATemMesmoPesoOuMaiorQueB(Heap *h, ItemHeap *a, ItemHeap *b, Analise *analise) {
  float notaAlunoA = ItemHeapObterNota(a);
  float notaAlunoB = ItemHeapObterNota(b);
  analise->comparacoes++;
  if (h->tipo == HEAP_MAX) {
    return notaAlunoA >= notaAlunoB;
  } else {
    return notaAlunoA <= notaAlunoB;
  }
}

bool HeapItemAPesoMaiorQueB(Heap *h, ItemHeap *a, ItemHeap *b, Analise *analise) {

  float notaAlunoA = ItemHeapObterNota(a);
  float notaAlunoB = ItemHeapObterNota(b);
  analise->comparacoes++;
  if (h->tipo == HEAP_MAX) {
    return notaAlunoA > notaAlunoB;
  } else {
    return notaAlunoA < notaAlunoB;
  }
}

void HeapEsvaziar(Heap *h, Analise *analise) {
  printf("\nRemoção inicializada!\n\n");
  ItemHeap removido;
  int qtdRemovidos = 0;
  while (HeapRemove(h, &removido, analise)) {
    qtdRemovidos++;
    ItemHeapImprime(&removido);
  }
  printf("\nRemoção finalizada | Foram removidos: %d\n", qtdRemovidos);
}

void HeapImprimeValidade(Heap *h, Analise *analise) {
  printf("\n%s valido!\n", HeapVerificaSeEhValido(h, analise) ? "É" : "Não é");
}

void HeapRemoverDesmarcados(Heap *heap) { heap->qtdItens = 0; }