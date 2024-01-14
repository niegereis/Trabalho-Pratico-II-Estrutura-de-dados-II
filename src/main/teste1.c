#include "../heap/heap.h"

int popularHeapT1(Heap *h, Analise *analise) {
  int qtd = 0;
  Aluno a = AlunoCriar("1", 0.5, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 1.0, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 11, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 5, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 9, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 8, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 4, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 5, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 2, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 0.5, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 12, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 3, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 10, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 10.5, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  a = AlunoCriar("1", 11.2, "MG", "SP", "COMP");
  HeapInserir(h, &a, analise);
  qtd++;

  return qtd;
}

int teste1() {
  Heap h = HeapCriar(15, HEAP_MIN);
  Analise analise = AnaliseCriar();

  int qtd = popularHeapT1(&h, &analise);
  printf("\nInserção finalizada!\n");
  HeapImprime(&h);
  printf("\nInserção finalizada | Foram inseridos %d items!\n", qtd);

  printf("/==============================/");
  HeapEsvaziar(&h, &analise);

  HeapImprime(&h);
  popularHeapT1(&h, &analise);
  HeapImprime(&h);

  printf("\n\nDesmarcando e reconstituindo");
  popularHeapT1(&h, &analise);
  HeapDesmarcarTodosEReconstituir(&h, &analise);
  HeapImprimeValidade(&h, &analise);
  HeapImprime(&h);

  HeapEsvaziar(&h, &analise);
  return 0;
}