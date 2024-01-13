#include "./intercalacaoBalanceadaFM1.h"

char *FitaObterCaminhoPelaPosicao2F(int p) {
  char *caminhoArquivo = malloc(sizeof(char) * 54);
  strncpy(caminhoArquivo, "./src/intercalacaoBalanceadaFM1/fitas2F/fita_", 46);

  // converte numero para string
  char stringNumero[3];
  sprintf(stringNumero, "%d", p);

  // concatena a palavra fita_ com o valor textual da variável i
  strncat(caminhoArquivo, stringNumero, 1);

  // concatena .txt a palavra fita_<i>
  strncat(caminhoArquivo, ".bin", 5);
  return caminhoArquivo;
}

void F2DefinirFitasDeSaida(Fita *fitas) {
  for (int i = ceil(QTD_FITAS / 2); i < QTD_FITAS; i++)
    fitas[i].ehSaida = true;
}

int FM2ObterFitaInicial(Fita *fitas, TipoDeFita tipo) { return tipo == FITA_DE_ENTRADA ? 0 : floor(QTD_FITAS / 2); }

int FM2ObterFitaFinal(Fita *fitas, TipoDeFita tipo) { return tipo == FITA_DE_ENTRADA ? floor(QTD_FITAS / 2) - 1 : QTD_FITAS - 1; }

int FM2ObterProximaFitaDoTipo(Fita *fitas, TipoDeFita tipo, int fitaAtual) {
  int fitaInicialDoTipo = FM2ObterFitaInicial(fitas, tipo);
  int fitaFinalDoTipo = FM2ObterFitaFinal(fitas, tipo);

  fitaAtual++;
  if (fitaAtual >= fitaFinalDoTipo)
    fitaAtual = fitaInicialDoTipo;

  return fitaAtual;
}

bool FM2JuntarNaFitaDe(Fita *fitas, TipoDeFita tipo, Analise *analise) {
  int fitaInicialDoOutroTipo = FM2ObterFitaInicial(fitas, tipo == FITA_DE_ENTRADA ? FITA_DE_SAIDA : FITA_DE_ENTRADA);
  int fitaFinalDoOutroTipo = FM2ObterFitaFinal(fitas, tipo == FITA_DE_ENTRADA ? FITA_DE_SAIDA : FITA_DE_ENTRADA);

  Heap heap = HeapCriar(QTD_FITAS, HEAP_TYPE);
  int fitaAtual = FM2ObterFitaInicial(fitas, tipo);

  int qtdTotalBlocosFormados = 0;
  while (true) {
    FM1Bloco *blocos = malloc(sizeof(FM1Bloco) * QTD_FITAS);
    int qtdNovoBloco = 0;
    int qtdBlocosLidos = 0;
    for (int i = fitaInicialDoOutroTipo; i <= fitaFinalDoOutroTipo; i++) {
      bool leuBloco = FM1BlocoLerViaArquivoBinario(fitas[i].arquivo, &blocos[i]);

      if (!leuBloco)
        continue;

      analise->transferenciasLeitura++;

      qtdBlocosLidos++;
      qtdNovoBloco += blocos[i].qtdItens;

      Aluno aluno;
      bool leuAluno = AlunoLerViaArquivoBinario(fitas[i].arquivo, &aluno);
      if (!leuAluno)
        continue;

      blocos[i].posicaoAtualNoBloco = 1;
      HeapInserirComFitaOrigem(&heap, &aluno, i, analise);
      analise->transferenciasLeitura++;
    }

    if (qtdBlocosLidos == 0)
      break;

    FM1Bloco novoBloco = FM1BlocoCriar(qtdNovoBloco);
    while (true) {
      Aluno aluno;
      ItemHeap itemRemovido;
      bool removeu = HeapRemove(&heap, &itemRemovido, analise);
      if (!removeu)
        break;

      FM1BlocoInserirAluno(&novoBloco, &itemRemovido.aluno);

      if (blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco == blocos[itemRemovido.fitaDeOrigem].qtdItens)
        continue;

      AlunoLerViaArquivoBinario(fitas[itemRemovido.fitaDeOrigem].arquivo, &aluno);
      blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco++;
      analise->transferenciasLeitura++;

      HeapInserirComFitaOrigem(&heap, &aluno, itemRemovido.fitaDeOrigem, analise);
    }

    qtdTotalBlocosFormados++;
    int escritas = FM1BlocoEscreverEmFita(&fitas[fitaAtual], &novoBloco);
    analise->transferenciasEscrita += escritas;
    if (DEBUG)
      printf("!Bloco inserido na fita %d com %d itens\n", fitaAtual, novoBloco.qtdItens);
    fitaAtual = FM2ObterProximaFitaDoTipo(fitas, tipo, fitaAtual);
  }

  return qtdTotalBlocosFormados == 1;
}

void IntercalacaoBalanceada(EstrategiaDeIntercalacao estrategia, int linhasALer, Analise *analise) {
  struct timespec inicio, fim;

  clock_gettime(CLOCK_MONOTONIC, &inicio);
  Fita *fitas = FM1GerarBlocos(linhasALer, estrategia, analise);
  if (fitas == NULL) {
    printf("Ocorreu um erro inesperado!");
    exit(1);
  }
  printf("INTERCALANDO...\n");

  bool finalizou = false;

  if (estrategia == FM1) {
    while (true) {

      FM1FitaResetarArquivos(fitas);
      bool x = FM1JuntarNaFitaDeSaida(fitas, analise);
      FitaRegerarFitas(fitas, FITA_DE_ENTRADA);

      if (x)
        break;

      FM1FitaResetarArquivos(fitas);
      FM1EspalharBlocosDaSaida(fitas, analise);
      FitaRegerarFitas(fitas, FITA_DE_SAIDA);
    }
  } else {
    while (true) {
      FM1FitaResetarArquivos(fitas);
      finalizou = FM2JuntarNaFitaDe(fitas, FITA_DE_SAIDA, analise);
      FitaRegerarFitas(fitas, FITA_DE_ENTRADA);

      if (finalizou)
        break;

      FM1FitaResetarArquivos(fitas);
      finalizou = FM2JuntarNaFitaDe(fitas, FITA_DE_ENTRADA, analise);
      FitaRegerarFitas(fitas, FITA_DE_SAIDA);

      if (finalizou)
        break;
    }
  }
  FM1FitaFecharArquivos(fitas);
  clock_gettime(CLOCK_MONOTONIC, &fim);
  AnaliseDefinirTempoPeloInicioEFim(analise, inicio, fim);
}