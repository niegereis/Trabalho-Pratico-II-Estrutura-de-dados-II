#include "./intercalacaoBalanceadaFM1.h"
#define QTD_FITAS 10
#define HEAP_TYPE HEAP_MIN

void FM1RegerarFitas(Fita *fitas, TipoDeFita tipo) {
  for (int i = 0; i < QTD_FITAS; i++) {
    if ((tipo == SAIDA && !fitas[i].ehSaida) || (tipo == ENTRADA && fitas[i].ehSaida))
      continue;

    char *caminhoArquivo = FitaObterCaminhoPelaPosicao(i);
    fclose(fitas[i].arquivo);

    FILE *arquivo = fopen(caminhoArquivo, "w+b");
    fitas[i].arquivo = arquivo;
  }
}

char *FitaObterCaminhoPelaPosicao(int p) {
  char *caminhoArquivo = malloc(sizeof(char) * 52);
  strncpy(caminhoArquivo, "./src/intercalacaoBalanceadaFM1/fitas/fita_", 44);

  // converte numero para string
  char stringNumero[3];
  sprintf(stringNumero, "%d", p);

  // concatena a palavra fita_ com o valor textual da variável i
  strncat(caminhoArquivo, stringNumero, 1);

  // concatena .txt a palavra fita_<i>
  strncat(caminhoArquivo, ".bin", 5);
  return caminhoArquivo;
}

Fita *FM1GerarFitas(int qtd) {
  if (!opendir("./src/intercalacaoBalanceadaFM1/fitas/")) {
    mkdir("./src/intercalacaoBalanceadaFM1/fitas/", 0777);
  }

  Fita *fitas = malloc(sizeof(Fita) * qtd);

  for (int i = 0; i < qtd; i++) {

    bool ehAUltimaFita = i == qtd - 1;
    char *caminhoArquivo = FitaObterCaminhoPelaPosicao(i);
    FILE *arquivo = fopen(caminhoArquivo, "w+b");
    fitas[i] = FitaCriar(arquivo, ehAUltimaFita ? true : false);

    for (int i = 0; i < 52; i++)
      caminhoArquivo[i] = '\0';

    free(caminhoArquivo);
  }

  return fitas;
}

void FM1DefinirFitaSaida(Fita *fitas, int qtd) { fitas[qtd - 1].ehSaida = true; }

Fita *FM1GerarBlocos(int qtdLinhas) {
  FILE *arquivoProvao = fopen("./PROVAO.TXT", "r");
  if (arquivoProvao == NULL) {
    printf("Não foi possível abrir o arquivo PROVAO.TXT");
    return NULL;
  }

  Fita *fitas = FM1GerarFitas(QTD_FITAS);
  FM1DefinirFitaSaida(fitas, QTD_FITAS);

  Heap heap = HeapCriar(10, HEAP_TYPE);

  int qtdAlunosLidos = 0;
  while (!HeapCheio(&heap)) {
    Aluno a = AlunoLer(arquivoProvao);
    qtdAlunosLidos++;
    HeapInserir(&heap, &a);
    // printf("LIDO (%d): ", qtdAlunosLidos);
    // AlunoImprime(&a);
    // if (foiMarcado)
    //   printf(" (Foi marcado)");

    // printf("\n");
  }
  // printf("\n");

  while (true) {

    int fitaSelecionada = rand() % QTD_FITAS;
    while (fitas[fitaSelecionada].ehSaida)
      fitaSelecionada = rand() % QTD_FITAS;

    // printf("\nFITA SELECIONADA %d\n\n", fitaSelecionada);
    // HeapImprime(&heap);
    // printf("\n\n");

    FM1Bloco bloco = FM1BlocoCriar(10000);
    bool removeu;
    do {
      // if (heap.ultimoRemovido != NULL) {
      //   printf("ULTIMO REMOVIDO: ");
      //   AlunoImprime(&(heap.ultimoRemovido->aluno));
      //   printf("\n");
      // }

      ItemHeap itemRemovido;
      removeu = HeapRemove(&heap, &itemRemovido);
      if (removeu) {
        // printf("REMOVIDO DO HEAP E INSERIDO NO BLOCO: ");
        // AlunoImprime(&itemRemovido.aluno);
        // printf("\n");

        FM1BlocoInserirAluno(&bloco, &itemRemovido.aluno);
      } else if (heap.ultimoRemovido != NULL) {
        break;
      }

      // printf("\n");

      if (feof(arquivoProvao) || qtdAlunosLidos >= qtdLinhas) {
        // HeapImprime(&heap);

        // printf("\n HEAP ITENS %d / HEAP MARCADOS %d\n", heap.qtdItens, heap.qtdItensMarcados);
        while (HeapRemove(&heap, &itemRemovido))
          FM1BlocoInserirAluno(&bloco, &itemRemovido.aluno);

        if (bloco.qtdItens > 0) {
          fitaSelecionada = rand() % QTD_FITAS;
          while (fitas[fitaSelecionada].ehSaida)
            fitaSelecionada = rand() % QTD_FITAS;
          FM1BlocoEscreverEmFita(&fitas[fitaSelecionada], &bloco);
          printf("!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, bloco.qtdItens);
        }

        HeapRemoverDesmarcados(&heap);
        HeapDesmarcarTodosEReconstituir(&heap);

        FM1Bloco blocoDosMarcadosRestantes = FM1BlocoCriar(10000);
        while (HeapRemove(&heap, &itemRemovido))
          FM1BlocoInserirAluno(&blocoDosMarcadosRestantes, &itemRemovido.aluno);
        if (blocoDosMarcadosRestantes.qtdItens > 0) {
          fitaSelecionada = rand() % QTD_FITAS;
          while (fitas[fitaSelecionada].ehSaida)
            fitaSelecionada = rand() % QTD_FITAS;
          FM1BlocoEscreverEmFita(&fitas[fitaSelecionada], &blocoDosMarcadosRestantes);
          printf("!!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, blocoDosMarcadosRestantes.qtdItens);
        }
        // printf("\n QTD ITENS DO BLOCOS: %d\n", bloco.qtdItens);
        printf("Alunos lidos %d\n", qtdAlunosLidos);

        return fitas;
      }

      if (HeapCheio(&heap))
        break;

      Aluno a = AlunoLer(arquivoProvao);
      qtdAlunosLidos++;
      // printf("LIDO (%d): ", qtdAlunosLidos);
      // AlunoImprime(&a);

      HeapInserir(&heap, &a);
      // if (foiMarcado)
      //   printf(" (Foi marcado)");
      // printf("\n\n");
      // HeapImprime(&heap);
    } while (removeu);

    // FM1BlocoImprimir(&bloco);
    FM1BlocoEscreverEmFita(&fitas[fitaSelecionada], &bloco);
    printf("!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, bloco.qtdItens);
    // printf("\nANTES DE RESCONSTITUIR E DESMARCAR (POS %d)", HeapObterPosicaoPrimeiroItemMarcado(&heap));
    // printf("\n HEAP ITENS %d / HEAP MARCADOS %d\n", heap.qtdItens, heap.qtdItensMarcados);
    // HeapImprime(&heap);
    HeapDesmarcarTodosEReconstituir(&heap);
    // printf("\nDEPOIS DE RESCONSTITUIR E DESMARCAR");
    // printf("\n HEAP ITENS %d / HEAP MARCADOS %d\n", heap.qtdItens, heap.qtdItensMarcados);
    // HeapImprime(&heap);
    // printf("\n");
  }

  return fitas;
}

bool FM1JuntarNaFitaDeSaida(Fita *fitas) {
  Heap heap = HeapCriar(QTD_FITAS, HEAP_TYPE);
  FM1Bloco *blocos = malloc(sizeof(FM1Bloco) * QTD_FITAS);
  int qtdBlocosEscritos = 0;
  do {
    int qtdNovoBloco = 0;
    int qtdBlocosLidos = 0;
    for (int i = 0; i < QTD_FITAS; i++) {
      int leuBloco = FM1BlocoLerViaArquivoBinario(fitas[i].arquivo, &blocos[i]);
      if (!leuBloco)
        continue;

      blocos[i].posicaoAtualNoBloco = 0;
      qtdNovoBloco += blocos[i].qtdItens;
      qtdBlocosLidos++;
      Aluno aluno;
      bool leuAluno = AlunoLerViaArquivoBinario(fitas[i].arquivo, &aluno);
      if (leuAluno) {
        blocos[i].posicaoAtualNoBloco++;
        HeapInserirComFitaOrigem(&heap, &aluno, i);
      }
    }
    if (qtdBlocosLidos == 0)
      break;

    // HeapImprime(&heap);

    bool removeu = true;
    FM1Bloco novoBloco = FM1BlocoCriar(qtdNovoBloco);
    ItemHeap itemRemovido;
    do {
      removeu = HeapRemove(&heap, &itemRemovido);
      if (removeu) {
        Aluno aluno;
        FM1BlocoInserirAluno(&novoBloco, &itemRemovido.aluno);
        if (blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco >= blocos[itemRemovido.fitaDeOrigem].qtdItens) {
          continue;
        }
        blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco++;

        bool leuNovoAluno = AlunoLerViaArquivoBinario(fitas[itemRemovido.fitaDeOrigem].arquivo, &aluno);
        if (leuNovoAluno)
          HeapInserirComFitaOrigem(&heap, &aluno, itemRemovido.fitaDeOrigem);
      }
    } while (removeu);
    // FM1BlocoImprimir(&novoBloco);
    int saida = FM1ObterFitaDeSaida(fitas);

    printf("Arquivo de saida recebeu bloco com %d registros em %d\n", novoBloco.qtdItens, saida);
    FM1BlocoEscreverEmFita(&fitas[saida], &novoBloco);
    qtdBlocosEscritos++;
  } while (true);
  return qtdBlocosEscritos == 1;
}

void FM1EspalharBlocosDaSaida(Fita *fitas) {
  int idxFitaSaida = FM1ObterFitaDeSaida(fitas);

  while (true) {
    int idxFitaSelecionada = rand() % QTD_FITAS;
    while (fitas[idxFitaSelecionada].ehSaida)
      idxFitaSelecionada = rand() % QTD_FITAS;

    Fita fitaSaida = fitas[idxFitaSaida];

    FM1Bloco bloco;
    bool leuBloco = FM1BlocoLerViaArquivoBinario(fitaSaida.arquivo, &bloco);
    if (!leuBloco)
      return;
    bloco.posicaoAtualNoBloco = 0;

    fwrite(&bloco, sizeof(FM1Bloco), 1, fitas[idxFitaSelecionada].arquivo);
    printf("BLOCO (%d): %d\n", idxFitaSelecionada, bloco.qtdItens);
    Aluno aluno;
    do {
      int itensLidos = fread(&aluno, sizeof(Aluno), 1, fitaSaida.arquivo);
      bloco.posicaoAtualNoBloco += itensLidos;
      fwrite(&aluno, sizeof(Aluno), 1, fitas[idxFitaSelecionada].arquivo);
      if (itensLidos == 0)
        break;

    } while (bloco.posicaoAtualNoBloco < bloco.qtdItens);
    printf("\nForam inseridos %d alunos na %i\n", bloco.posicaoAtualNoBloco, idxFitaSelecionada);
  }
}

FM1Bloco FM1BlocoCriar(int qtdMax) {
  FM1Bloco bloco;
  bloco.alunos = malloc(sizeof(Aluno) * qtdMax);
  bloco.qtdItens = 0;
  bloco.qtdMax = qtdMax;
  bloco.posicaoAtualNoBloco = 0;
  return bloco;
}

bool FM1BlocoInserirAluno(FM1Bloco *bloco, Aluno *a) {
  if (bloco->qtdMax == bloco->qtdItens)
    return false;

  bloco->alunos[bloco->qtdItens] = *a;
  bloco->qtdItens++;
  return true;
}

void FM1BlocoEscreverEmFita(Fita *fita, FM1Bloco *bloco) {
  fwrite(bloco, sizeof(FM1Bloco), 1, fita->arquivo);
  for (int i = 0; i < bloco->qtdItens; i++) {
    fwrite(&(bloco->alunos[i]), sizeof(Aluno), 1, fita->arquivo);
  }
  return;
}

void FM1BlocoImprimir(FM1Bloco *bloco) {
  printf("\n=================================BLOCO %d================================\n", bloco->qtdItens);
  for (int i = 0; i < bloco->qtdItens; i++) {
    AlunoImprimeCompleto(&bloco->alunos[i]);
    printf("\n");
  }
  printf("\n========================================================================\n");
}

bool FM1BlocoLerViaArquivoBinario(FILE *arquivo, FM1Bloco *bloco) { return fread(bloco, sizeof(FM1Bloco), 1, arquivo) == 1; }

int FM1ObterFitaDeSaida(Fita *fitas) {
  for (int i = 0; i < QTD_FITAS; i++)
    if (fitas[i].ehSaida)
      return i;

  return -1;
}

void FM1FitaFecharArquivos(Fita *fitas) {
  for (int i = 0; i < QTD_FITAS; i++)
    fclose(fitas[i].arquivo);
}

void FM1FitaResetarArquivos(Fita *fitas) {
  for (int i = 0; i < QTD_FITAS; i++)
    fseek(fitas[i].arquivo, 0, 0);
}

bool FM1FinalizouOProcesso(Fita *fitas) {
  int idxFitaSaida = FM1ObterFitaDeSaida(fitas);
  FM1Bloco bloco;
  int qtdBlocosLidos = 0;
  bool leuBloco;
  do {
    leuBloco = FM1BlocoLerViaArquivoBinario(fitas[idxFitaSaida].arquivo, &bloco);
    if (!leuBloco)
      continue;

    bloco.posicaoAtualNoBloco = 0;
    qtdBlocosLidos++;

    Aluno aluno;
    bool leuAluno = AlunoLerViaArquivoBinario(fitas[idxFitaSaida].arquivo, &aluno);
    if (leuAluno)
      bloco.posicaoAtualNoBloco++;

    if (qtdBlocosLidos > 1)
      return false;
  } while (leuBloco);

  return qtdBlocosLidos == 1;
}