#include "../heap/heap.h"

int popularHeapT2(Heap *h, Analise *analise) {
  int qtd = 0;

  Aluno a = AlunoCriar("1", 3, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  ItemHeap i;
  HeapRemove(h, &i, analise);

  a = AlunoCriar("1", 2, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 3, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  return qtd;
}

int teste2() {
  Heap h = HeapCriar(3, HEAP_MAX);
  Analise analise = AnaliseCriar();

  int qtd = popularHeapT2(&h, &analise);
  printf("\nInserção finalizada!\n");
  HeapImprime(&h);
  printf("\nInserção finalizada | Foram inseridos %d items!\n", qtd);

  HeapEsvaziar(&h, &analise);
  return 0;
}
