#include "../lib/heap.h"
#include "../lib/intercalacaoBalanceada.h"

char *F2FitaObterCaminhoPelaPosicao(int p) {
  char *caminhoArquivo = malloc(sizeof(char) * 54);
  strncpy(caminhoArquivo, "./fitas/fitas2F/fita_", 46);

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
  for (int i = ceil(QTD_FITAS_2F / 2); i < QTD_FITAS_2F; i++)
    fitas[i].ehSaida = true;
}

int F2ObterFitaInicial(TipoDeFita tipo) { return tipo == FITA_DE_ENTRADA ? 0 : floor(QTD_FITAS_2F / 2); }

int F2ObterFitaFinal(TipoDeFita tipo) { return tipo == FITA_DE_ENTRADA ? floor(QTD_FITAS_2F / 2) - 1 : QTD_FITAS_2F - 1; }

int F2ObterProximaFitaDoTipo(TipoDeFita tipo, int fitaAtual) {
  int fitaInicialDoTipo = F2ObterFitaInicial(tipo);
  int fitaFinalDoTipo = F2ObterFitaFinal(tipo);

  fitaAtual++;
  if (fitaAtual >= fitaFinalDoTipo)
    fitaAtual = fitaInicialDoTipo;

  return fitaAtual;
}

bool F2JuntarNaFitaDe(Fita *fitas, TipoDeFita tipo, Analise *analise) {
  int fitaInicialDoOutroTipo = F2ObterFitaInicial(tipo == FITA_DE_ENTRADA ? FITA_DE_SAIDA : FITA_DE_ENTRADA);
  int fitaFinalDoOutroTipo = F2ObterFitaFinal(tipo == FITA_DE_ENTRADA ? FITA_DE_SAIDA : FITA_DE_ENTRADA);

  Heap heap = HeapCriar(QTD_FITAS_2F, HEAP_TYPE);
  int fitaAtual = F2ObterFitaInicial(tipo);

  int qtdTotalBlocosFormados = 0;
  while (true) {
    Bloco *blocos = malloc(sizeof(Bloco) * QTD_FITAS_2F);
    int qtdNovoBloco = 0;
    int qtdBlocosLidos = 0;
    for (int i = fitaInicialDoOutroTipo; i <= fitaFinalDoOutroTipo; i++) {
      bool leuBloco = BlocoLerViaArquivoBinario(fitas[i].arquivo, &blocos[i]);

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

    Bloco novoBloco = BlocoCria(qtdNovoBloco);
    while (true) {
      Aluno aluno;
      ItemHeap itemRemovido;
      bool removeu = HeapRemove(&heap, &itemRemovido, analise);
      if (!removeu)
        break;

      BlocoInserirAluno(&novoBloco, &itemRemovido.aluno);

      if (blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco == blocos[itemRemovido.fitaDeOrigem].qtdItens)
        continue;

      AlunoLerViaArquivoBinario(fitas[itemRemovido.fitaDeOrigem].arquivo, &aluno);
      blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco++;
      analise->transferenciasLeitura++;

      HeapInserirComFitaOrigem(&heap, &aluno, itemRemovido.fitaDeOrigem, analise);
    }

    qtdTotalBlocosFormados++;
    int escritas = BlocoEscreverEmFita(&fitas[fitaAtual], &novoBloco);
    analise->transferenciasEscrita += escritas;
    if (DEBUG)
      printf("!Bloco inserido na fita %d com %d itens\n", fitaAtual, novoBloco.qtdItens);
    fitaAtual = F2ObterProximaFitaDoTipo(tipo, fitaAtual);
  }

  return qtdTotalBlocosFormados == 1;
}
