#include "./intercalacaoBalanceadaFM1.h"

void FitaRegerarFitas(Fita *fitas, TipoDeFita tipo) {
  for (int i = 0; i < QTD_FITAS; i++) {
    if ((tipo == FITA_DE_SAIDA && !fitas[i].ehSaida) || (tipo == FITA_DE_ENTRADA && fitas[i].ehSaida))
      continue;

    char *caminhoArquivo = FitaObterCaminhoPelaPosicaoFM1(i);
    fclose(fitas[i].arquivo);

    FILE *arquivo = fopen(caminhoArquivo, "w+b");
    fitas[i].arquivo = arquivo;
  }
}

char *FitaObterCaminhoPelaPosicaoFM1(int p) {
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

Fita *FitaGerarFitas(int qtd) {
  if (!opendir("./src/intercalacaoBalanceadaFM1/fitas/")) {
    mkdir("./src/intercalacaoBalanceadaFM1/fitas/", 0777);
  }

  Fita *fitas = malloc(sizeof(Fita) * qtd);

  for (int i = 0; i < qtd; i++) {

    char *caminhoArquivo = FitaObterCaminhoPelaPosicaoFM1(i);
    FILE *arquivo = fopen(caminhoArquivo, "w+b");
    fitas[i] = FitaCriar(arquivo, false);

    for (int i = 0; i < 52; i++)
      caminhoArquivo[i] = '\0';

    free(caminhoArquivo);
  }

  return fitas;
}

void FM1DefinirFitaSaida(Fita *fitas) { fitas[QTD_FITAS - 1].ehSaida = true; }

int FitaObterNumeroDeFitaDeEntradas(Fita *f) {
  int c = 0;
  for (int i = 0; i < QTD_FITAS; i++) {
    if (!f[i].ehSaida)
      c++;
  }
  return c;
}

Fita *FM1GerarBlocos(int qtdLinhas, EstrategiaDeIntercalacao estrategia, Analise *analise) {
  FILE *arquivoProvao = fopen("./PROVAO.TXT", "r");
  if (arquivoProvao == NULL) {
    printf("Não foi possível abrir o arquivo PROVAO.TXT");
    return NULL;
  }

  Fita *fitas = FitaGerarFitas(QTD_FITAS);
  if (estrategia == FM1)
    FM1DefinirFitaSaida(fitas);
  else
    F2DefinirFitasDeSaida(fitas);

  Heap heap = HeapCriar(10, HEAP_TYPE);

  int qtdAlunosLidos = 0;
  while (!HeapCheio(&heap) && qtdAlunosLidos < qtdLinhas) {
    Aluno a = AlunoLer(arquivoProvao);
    analise->transferenciasLeitura++;

    qtdAlunosLidos++;
    HeapInserir(&heap, &a, analise);
  }

  int fitaSelecionada = 0;
  while (true) {

    FM1Bloco bloco = FM1BlocoCriar(10000);
    bool removeu;
    do {

      ItemHeap itemRemovido;
      removeu = HeapRemove(&heap, &itemRemovido, analise);
      if (removeu) {

        FM1BlocoInserirAluno(&bloco, &itemRemovido.aluno);
      } else if (heap.ultimoRemovido != NULL) {
        break;
      }

      if (feof(arquivoProvao) || qtdAlunosLidos >= qtdLinhas) {

        while (HeapRemove(&heap, &itemRemovido, analise))
          FM1BlocoInserirAluno(&bloco, &itemRemovido.aluno);

        if (bloco.qtdItens > 0) {
          int escritas = FM1BlocoEscreverEmFita(&fitas[fitaSelecionada], &bloco);
          analise->transferenciasEscrita += escritas;
          if (DEBUG)
            printf("!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, bloco.qtdItens);
          fitaSelecionada = (fitaSelecionada + 1) % FitaObterNumeroDeFitaDeEntradas(fitas);
        }

        HeapRemoverDesmarcados(&heap);
        HeapDesmarcarTodosEReconstituir(&heap, analise);

        FM1Bloco blocoDosMarcadosRestantes = FM1BlocoCriar(10000);
        while (HeapRemove(&heap, &itemRemovido, analise))
          FM1BlocoInserirAluno(&blocoDosMarcadosRestantes, &itemRemovido.aluno);
        if (blocoDosMarcadosRestantes.qtdItens > 0) {
          int escritas = FM1BlocoEscreverEmFita(&fitas[fitaSelecionada], &blocoDosMarcadosRestantes);
          analise->transferenciasEscrita += escritas;
          if (DEBUG)
            printf("!!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, blocoDosMarcadosRestantes.qtdItens);
        }
        if (DEBUG)
          printf("Alunos lidos %d\n", qtdAlunosLidos);

        return fitas;
      }

      if (HeapCheio(&heap))
        break;

      Aluno a = AlunoLer(arquivoProvao);
      analise->transferenciasLeitura++;
      qtdAlunosLidos++;

      HeapInserir(&heap, &a, analise);

    } while (removeu);

    int escritas = FM1BlocoEscreverEmFita(&fitas[fitaSelecionada], &bloco);
    analise->transferenciasEscrita += escritas;
    if (DEBUG)
      printf("!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, bloco.qtdItens);
    fitaSelecionada = (fitaSelecionada + 1) % FitaObterNumeroDeFitaDeEntradas(fitas);

    HeapDesmarcarTodosEReconstituir(&heap, analise);
  }

  return fitas;
}

bool FM1JuntarNaFitaDeSaida(Fita *fitas, Analise *analise) {
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

      analise->transferenciasLeitura++;

      blocos[i].posicaoAtualNoBloco = 0;
      qtdNovoBloco += blocos[i].qtdItens;
      qtdBlocosLidos++;

      Aluno aluno;
      bool leuAluno = AlunoLerViaArquivoBinario(fitas[i].arquivo, &aluno);
      if (!leuAluno)
        continue;

      analise->transferenciasLeitura++;
      blocos[i].posicaoAtualNoBloco++;
      HeapInserirComFitaOrigem(&heap, &aluno, i, analise);
    }
    if (qtdBlocosLidos == 0)
      break;

    bool removeu = true;
    FM1Bloco novoBloco = FM1BlocoCriar(qtdNovoBloco);
    ItemHeap itemRemovido;
    do {
      removeu = HeapRemove(&heap, &itemRemovido, analise);
      if (removeu) {
        Aluno aluno;
        FM1BlocoInserirAluno(&novoBloco, &itemRemovido.aluno);
        if (blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco >= blocos[itemRemovido.fitaDeOrigem].qtdItens) {
          continue;
        }
        blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco++;

        bool leuNovoAluno = AlunoLerViaArquivoBinario(fitas[itemRemovido.fitaDeOrigem].arquivo, &aluno);
        if (!leuNovoAluno)
          continue;

        HeapInserirComFitaOrigem(&heap, &aluno, itemRemovido.fitaDeOrigem, analise);
        analise->transferenciasLeitura++;
      }
    } while (removeu);
    // FM1BlocoImprimir(&novoBloco);
    int saida = FM1ObterFitaDeSaida(fitas);

    if (DEBUG)
      printf("Arquivo de saida recebeu bloco com %d registros em %d\n", novoBloco.qtdItens, saida);

    int escritas = FM1BlocoEscreverEmFita(&fitas[saida], &novoBloco);
    analise->transferenciasEscrita += escritas;

    qtdBlocosEscritos++;
  } while (true);
  return qtdBlocosEscritos == 1;
}

void FM1EspalharBlocosDaSaida(Fita *fitas, Analise *analise) {
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

    analise->transferenciasLeitura++;
    bloco.posicaoAtualNoBloco = 0;

    fwrite(&bloco, sizeof(FM1Bloco), 1, fitas[idxFitaSelecionada].arquivo);
    analise->transferenciasEscrita++;

    if (DEBUG)
      printf("BLOCO de saida (%d): %d\n", idxFitaSelecionada, bloco.qtdItens);
    Aluno aluno;
    do {
      int itensLidos = fread(&aluno, sizeof(Aluno), 1, fitaSaida.arquivo);
      bloco.posicaoAtualNoBloco += itensLidos;
      AlunoEscreverEmArquivoBin(&aluno, fitas[idxFitaSelecionada].arquivo);
      analise->transferenciasEscrita++;
      if (itensLidos == 0)
        break;

    } while (bloco.posicaoAtualNoBloco < bloco.qtdItens);
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

int FM1BlocoEscreverEmFita(Fita *fita, FM1Bloco *bloco) {
  fwrite(bloco, sizeof(FM1Bloco), 1, fita->arquivo);
  int escritas = 0;
  for (int i = 0; i < bloco->qtdItens; i++) {
    fwrite(&(bloco->alunos[i]), sizeof(Aluno), 1, fita->arquivo);
    escritas++;
  }
  return escritas;
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