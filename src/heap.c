#include "../lib/heap.h"

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

// Função usada para criar um heap
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

// Função usada para inserir no heap
bool HeapInserir(Heap *heap, Aluno *aluno, Analise *analise) {
  // Verifica se o heap está cheia
  if ((heap->qtdItens + heap->qtdItensMarcados) == heap->qtdMaxima)
    return false;

  // Cria um novo item para o aluno com uma fita de origem não especificada
  ItemHeap novoItem = ItemHeapCriar(aluno, -1);

  // Verifica se o novo item deve ser inserido como marcado
  bool ehParaInserirMarcado =
      heap->ultimoRemovido != NULL && HeapItemAPesoMaiorQueB(heap, &novoItem, heap->ultimoRemovido, analise);

  // Insere o novo item na heap, marcado ou não, de acordo com a decisão anterior
  if (ehParaInserirMarcado) {
    novoItem.marcado = true;

    // Calcula a posição para inserir o novo item marcado
    int posicaoNovoItemMarcado = heap->qtdMaxima - heap->qtdItensMarcados - 1;
    heap->itensHeap[posicaoNovoItemMarcado] = novoItem;
    heap->qtdItensMarcados++;
  } else {
    // Insere o novo item não marcado e reconstitui a heap
    heap->itensHeap[heap->qtdItens++] = novoItem;
    for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
      HeapReconstituir(heap, i, analise);
  }

  // Indica que a inserção foi bem-sucedida
  return true;
}

// Função usada para inserir no heap com fita de origem especificada
bool HeapInserirComFitaOrigem(Heap *heap, Aluno *aluno, int fitaDeOrigem, Analise *analise) {
  // Verifica se o heap está cheio
  if (HeapCheio(heap))
    return false;

  // Cria um novo item para o aluno com a fita de origem especificada
  ItemHeap novoItem = ItemHeapCriar(aluno, fitaDeOrigem);
  // Insere o novo item na heap e reconstitui a heap
  heap->itensHeap[heap->qtdItens++] = novoItem;
  for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
    HeapReconstituir(heap, i, analise);

  // Indica que a inserção foi bem-sucedida
  return true;
}

// Função usada para verificar se o heap está cheio
bool HeapCheio(Heap *heap) { return heap->qtdItens + heap->qtdItensMarcados == heap->qtdMaxima; }

// Função usada para reconstituir o heap
void HeapReconstituir(Heap *heap, int pos, Analise *analise) {
  // Verifica se a posição é inválida
  if (pos < 0)
    return;

  // Calcula as posições dos filhos esquerdo e direito
  int esq = 2 * pos + 1;
  int dir = 2 * pos + 2;

  // Ajusta a posição do filho direito se estiver fora dos limites do vetor
  if (dir >= heap->qtdItens)
    dir--;

  // Verifica se a posição do filho esquerdo está além dos limites do vetor
  if (esq >= heap->qtdItens)
    return;

  // Obtém os itens nas posições do centro, esquerda e direita
  ItemHeap itemHeapCentro = heap->itensHeap[pos];
  ItemHeap itemHeapEsquerda = heap->itensHeap[esq];
  ItemHeap itemHeapDireita = heap->itensHeap[dir];

  // Verifica se a prioridade do filho direito é maior que a do filho esquerdo e do centro
  bool direitaPrioridadeSobreEsquerda = HeapItemAPesoMaiorQueB(heap, &itemHeapDireita, &itemHeapEsquerda, analise);
  bool direitaPrioridadeSobreCentro = HeapItemAPesoMaiorQueB(heap, &itemHeapDireita, &itemHeapCentro, analise);
  if (direitaPrioridadeSobreEsquerda && direitaPrioridadeSobreCentro) {
    // Troca a posição do centro com a posição do filho direito e continua a reconstituição
    HeapTroca(heap, pos, dir);
    HeapReconstituir(heap, dir, analise);
    return;
  }

  // Verifica se a prioridade do filho esquerdo é maior ou igual à do filho direito e do centro
  bool esquerdaMesmoPesoOuMaiorSobreDireita =
      HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapEsquerda, &itemHeapDireita, analise);
  bool esquerdaPrioridadeSobreCentro = HeapItemAPesoMaiorQueB(heap, &itemHeapEsquerda, &itemHeapCentro, analise);
  if (esquerdaMesmoPesoOuMaiorSobreDireita && esquerdaPrioridadeSobreCentro) {
    // Troca a posição do centro com a posição do filho esquerdo e continua a reconstituição
    HeapTroca(heap, pos, esq);
    HeapReconstituir(heap, esq, analise);
  }
}

// Função usada para desmarcar todos os itens marcados na heap e reconstituir o heap
void HeapDesmarcarTodosEReconstituir(Heap *heap, Analise *analise) {
  // Obtém a posição do primeiro item marcado no heap
  int posicaoPrimeiroItemMarcado = HeapObterPosicaoPrimeiroItemMarcado(heap);

  // Desmarca todos os itens marcados e reorganiza o heap
  for (int i = posicaoPrimeiroItemMarcado + 1; i < heap->qtdMaxima; i++) {
    // Desmarca o item
    heap->itensHeap[i].marcado = false;

    // Verifica se há espaço para mover o item para a parte não marcada do heap
    if (heap->qtdItens + heap->qtdItensMarcados < heap->qtdMaxima)
      heap->itensHeap[heap->qtdItens] = heap->itensHeap[i];

    // Atualiza as contagens de itens
    heap->qtdItens++;
    heap->qtdItensMarcados--;
  }

  // Reconstitui a propriedade de heap a partir do topo do heap
  for (int i = (heap->qtdItens / 2) - 1; i >= 0; i--)
    HeapReconstituir(heap, i, analise);
}

// Função que calcula e retorna a posição do primeiro item marcado na heap
int HeapObterPosicaoPrimeiroItemMarcado(Heap *heap) { return heap->qtdMaxima - heap->qtdItensMarcados - 1; }

// Função usada para trocar os elementos nas posições p1 e p2 do vetor do heap
void HeapTroca(Heap *heap, int p1, int p2) {
  ItemHeap aux = heap->itensHeap[p1];
  heap->itensHeap[p1] = heap->itensHeap[p2];
  heap->itensHeap[p2] = aux;
}

// Função usada para remover o elemento de maior prioridade (raiz) do heap e reconstitui a propriedade de heap
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

// Função usada para imprimir as informações contidas na estrutura ItemHeap
void ItemHeapImprime(ItemHeap *itemHeap) {
  AlunoImprime(&itemHeap->aluno);
  printf(" (%s marcado!)", itemHeap->marcado ? "Está" : "Não está");
  printf("\n");
}

// Função usada para imprimir o estado atual da heap, incluindo detalhes sobre os itens não marcados e marcados
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

// Função que verifica recursivamente se a propriedade de heap é mantida a partir de uma posição específica
bool HeapVerificaSeEhValidoRecursivo(Heap *heap, int centro, bool valido, Analise *analise) {
  // Verifica se a posição central está além da metade do vetor
  if (centro > floor(heap->qtdItens / 2) - 1)
    return true;

  // Calcula as posições dos filhos esquerdo e direito
  int esquerda = 2 * centro;
  int direita = 2 * centro + 1;
  // Ajusta a posição do filho direito se estiver fora dos limites do vetor
  if (direita >= heap->qtdItens)
    direita--;

  // Obtém os itens nas posições do centro, esquerda e direita
  ItemHeap itemHeapCentro = heap->itensHeap[centro];
  ItemHeap itemHeapEsquerda = heap->itensHeap[esquerda];
  ItemHeap itemHeapDireita = heap->itensHeap[direita];

  // Verifica se o centro tem prioridade sobre a direita e a esquerda
  bool centroTemPrioridadeADireita = HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapCentro, &itemHeapDireita, analise);
  bool centroPrioridadeEsquerda = HeapItemATemMesmoPesoOuMaiorQueB(heap, &itemHeapCentro, &itemHeapEsquerda, analise);

  // Verifica se a posição é válida
  bool posicaoValida = centroTemPrioridadeADireita && centroPrioridadeEsquerda;

  // Verifica recursivamente as posições à esquerda e à direita
  bool esquerdaValida = true;
  if (centro != esquerda)
    esquerdaValida = HeapVerificaSeEhValidoRecursivo(heap, esquerda, valido && posicaoValida, analise);

  bool direitaValida = HeapVerificaSeEhValidoRecursivo(heap, direita, valido && posicaoValida, analise);

  // Retorna true se ambas as verificações à esquerda e à direita forem válidas
  return direitaValida && esquerdaValida;
}

// Função usada para verificar se o heap é válido
bool HeapVerificaSeEhValido(Heap *heap, Analise *analise) { return HeapVerificaSeEhValidoRecursivo(heap, 0, true, analise); }

// Função usada para comparar dois itens da heap com base em suas notas, considerando o tipo da heap
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

// Função usada para comparar dois itens do heap considerando apenas se a nota do item A é estritamente maior que a nota do item B
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

// Função usada para remover todos os itens do heap e imprime detalhes sobre cada remoção
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

// Função usada para imprimir se o heap é válida ou não com base na propriedade de heap
void HeapImprimeValidade(Heap *h, Analise *analise) {
  printf("\n%s valido!\n", HeapVerificaSeEhValido(h, analise) ? "É" : "Não é");
}

void HeapRemoverDesmarcados(Heap *heap) { heap->qtdItens = 0; }