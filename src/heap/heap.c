
#include "./heap.h"

#include "heap.h"

void HeapTroca(Heap *heap, int p1, int p2);
void HeapReconstituir(Heap *heap, int pos);

ItemHeap ItemHeapCriar(Aluno *aluno) {
  ItemHeap itemHeap;
  itemHeap.aluno = *aluno;
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
  heap.tipo = tipo;
  return heap;
}

bool HeapInserir(Heap *heap, Aluno aluno, bool marcado) {
  if ((heap->qtdItens + heap->qtdItensMarcados) == heap->qtdMaxima)
    return false;

  ItemHeap novoItem = ItemHeapCriar(&aluno);

  bool ehParaInserirMarcado = HeapItemAPesoMaiorQueB(heap, &heap->ultimoRemovido, &novoItem);

  if (ehParaInserirMarcado || marcado) {
    novoItem.marcado = true;

    int posicaoNovoItemMarcado = heap->qtdMaxima - heap->qtdItensMarcados - 1;
    heap->itensHeap[posicaoNovoItemMarcado] = novoItem;
    heap->qtdItensMarcados++;
  } else {
    heap->itensHeap[heap->qtdItens++] = novoItem;
    for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
      HeapReconstituir(heap, i);
  }

  return true;
}

void HeapReconstituir(Heap *heap, int pos) {
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

  bool direitaPrioridadeSobreEsquerda = HeapItemAPesoMaiorQueB(heap, &itemHeapDireita, &itemHeapEsquerda);
  bool direitaPrioridadeSobreCentro = HeapItemAPesoMaiorQueB(heap, &itemHeapDireita, &itemHeapCentro);
  if (direitaPrioridadeSobreEsquerda && direitaPrioridadeSobreCentro) {
    HeapTroca(heap, pos, dir);
    HeapReconstituir(heap, dir);
  }

  bool esquerdaMesmoPesoOuMaiorSobreDireita = HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapEsquerda, &itemHeapDireita);
  bool esquerdaPrioridadeSobreCentro = HeapItemAPesoMaiorQueB(heap, &itemHeapEsquerda, &itemHeapCentro);
  if (esquerdaMesmoPesoOuMaiorSobreDireita && esquerdaPrioridadeSobreCentro) {
    HeapTroca(heap, pos, esq);
    HeapReconstituir(heap, esq);
  }
}

void HeapDesmarcarTodosEReconstituir(Heap *heap) {
  int posicaoPrimeiroItemMarcado = HeapObterPosicaoPrimeiroItemMarcado(heap);

  for (int i = posicaoPrimeiroItemMarcado; i < heap->qtdMaxima; i++) {
    heap->itensHeap[i].marcado = false;
    heap->qtdItens++;
    heap->qtdItensMarcados--;
  }

  for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
    HeapReconstituir(heap, i);
}

int HeapObterPosicaoPrimeiroItemMarcado(Heap *heap) { return heap->qtdMaxima - heap->qtdItensMarcados - 1; }
void HeapTroca(Heap *heap, int p1, int p2) {
  ItemHeap aux = heap->itensHeap[p1];
  heap->itensHeap[p1] = heap->itensHeap[p2];
  heap->itensHeap[p2] = aux;
}

bool HeapRemove(Heap *heap, ItemHeap *itemObtido) {
  if (heap->qtdItens == 0)
    return false;

  ItemHeap ultimoItem = heap->itensHeap[heap->qtdItens - 1];

  *itemObtido = heap->itensHeap[0];
  heap->ultimoRemovido = *itemObtido;

  heap->qtdItens--;

  heap->itensHeap[0] = ultimoItem;

  for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
    HeapReconstituir(heap, i);

  return true;
}

void ItemHeapImprime(ItemHeap *itemHeap) {
  AlunoImprime(&itemHeap->aluno);
  printf(" (%s marcado!)", itemHeap->marcado ? "Está" : "Não está");
  printf("\n");
}

void HeapImprime(Heap *heap) {
  int posicaoPrimeiroItemMarcado = HeapObterPosicaoPrimeiroItemMarcado(heap);
  printf("\n");

  for (int i = 0; i < heap->qtdMaxima; i++) {
    if (i < heap->qtdItens || i > posicaoPrimeiroItemMarcado)
      ItemHeapImprime(&heap->itensHeap[i]);
    else
      printf("... VAZIO ...\n");
  }
}

bool HeapVerificaSeEhValidoRecursivo(Heap *heap, int centro, bool valido) {
  if (centro > floor(heap->qtdItens / 2) - 1)
    return true;

  int esquerda = 2 * centro;
  int direita = 2 * centro + 1;
  if (direita >= heap->qtdItens)
    direita--;

  ItemHeap itemHeapCentro = heap->itensHeap[centro];
  ItemHeap itemHeapEsquerda = heap->itensHeap[esquerda];
  ItemHeap itemHeapDireita = heap->itensHeap[direita];

  bool centroTemPrioridadeADireita = HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapCentro, &itemHeapDireita);
  bool centroPrioridadeEsquerda = HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapCentro, &itemHeapEsquerda);

  bool posicaoValida = centroTemPrioridadeADireita && centroPrioridadeEsquerda;

  bool esquerdaValida = true;
  if (centro != esquerda)
    esquerdaValida = HeapVerificaSeEhValidoRecursivo(heap, esquerda, valido && posicaoValida);

  bool direitaValida = HeapVerificaSeEhValidoRecursivo(heap, direita, valido && posicaoValida);
  return direitaValida && esquerdaValida;
}

bool HeapVerificaSeEhValido(Heap *heap) { return HeapVerificaSeEhValidoRecursivo(heap, 0, true); }

bool HeapItemATemMesmoPesoOuMaiorQueB(Heap *h, ItemHeap *a, ItemHeap *b) {
  float notaAlunoA = ItemHeapObterNota(a);
  float notaAlunoB = ItemHeapObterNota(b);
  if (h->tipo == HEAP_MAX) {
    return notaAlunoA >= notaAlunoB;
  } else {
    return notaAlunoB <= notaAlunoA;
  }
}

bool HeapItemAPesoMaiorQueB(Heap *h, ItemHeap *a, ItemHeap *b) {
  float notaAlunoA = ItemHeapObterNota(a);
  float notaAlunoB = ItemHeapObterNota(b);
  if (h->tipo == HEAP_MAX) {
    return notaAlunoA > notaAlunoB;
  } else {
    return notaAlunoB < notaAlunoA;
  }
}

void HeapEsvaziar(Heap *h) {
  printf("\nRemoção inicializada!\n\n");
  ItemHeap removido;
  int qtdRemovidos = 0;
  while (HeapRemove(h, &removido)) {
    qtdRemovidos++;
    ItemHeapImprime(&removido);
  }
  printf("\nRemoção finalizada | Foram removidos: %d\n", qtdRemovidos);
}

void HeapImprimeValidade(Heap *h) { printf("\n%s valido!\n", HeapVerificaSeEhValido(h) ? "É" : "Não é"); }
