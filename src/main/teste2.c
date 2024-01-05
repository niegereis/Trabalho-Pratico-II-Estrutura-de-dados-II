#include "../heap/heap.h"

int popularHeapT2(Heap *h) {
  int qtd = 0;

  Aluno a = AlunoCriar(1, 3, "MG", "SP", "COMP");
  HeapInserir(h, a, false);
  qtd++;

  ItemHeap i;
  HeapRemove(h, &i);

  a = AlunoCriar(1, 2, "MG", "SP", "COMP");
  HeapInserir(h, a, false);
  qtd++;

  a = AlunoCriar(1, 3, "MG", "SP", "COMP");
  HeapInserir(h, a, false);
  qtd++;

  return qtd;
}

int teste2() {
  Heap h = HeapCriar(3, HEAP_MAX);

  int qtd = popularHeapT2(&h);
  printf("\nInserção finalizada!\n");
  HeapImprime(&h);
  printf("\nInserção finalizada | Foram inseridos %d items!\n", qtd);

  HeapEsvaziar(&h);
  return 0;
}
