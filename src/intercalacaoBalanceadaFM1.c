#include "../lib/heap.h"
#include "../lib/intercalacaoBalanceada.h"

void FM1DefinirFitaSaida(Fita *fitas) { fitas[QTD_FITAS_FM1 - 1].ehSaida = true; }

bool FM1JuntarNaFitaDeSaida(Fita *fitas, Analise *analise) {
  Heap heap = HeapCriar(QTD_FITAS_FM1 - 2, HEAP_TYPE);
  Bloco *blocos = malloc(sizeof(Bloco) * QTD_FITAS_FM1 - 1);
  int qtdBlocosEscritos = 0;
  do {
    int qtdNovoBloco = 0;
    int qtdBlocosLidos = 0;
    for (int i = 0; i < (QTD_FITAS_FM1 - 2); i++) {
      int leuBloco = BlocoLerViaArquivoBinario(fitas[i].arquivo, &blocos[i]);
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
    Bloco novoBloco = BlocoCria(qtdNovoBloco);
    ItemHeap itemRemovido;
    do {
      removeu = HeapRemove(&heap, &itemRemovido, analise);
      if (removeu) {
        Aluno aluno;
        BlocoInserirAluno(&novoBloco, &itemRemovido.aluno);
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
    int saida = FM1ObterFitaDeSaida(fitas);

    if (DEBUG)
      printf("Arquivo de saida recebeu bloco com %d registros em %d\n", novoBloco.qtdItens, saida);

    int escritas = BlocoEscreverEmFita(&fitas[saida], &novoBloco);
    analise->transferenciasEscrita += escritas;

    qtdBlocosEscritos++;
  } while (true);
  return qtdBlocosEscritos == 1;
}

void FM1EspalharBlocosDaSaida(Fita *fitas, Analise *analise) {
  int idxFitaSaida = FM1ObterFitaDeSaida(fitas);

  int idxFitaSelecionada = -1;
  while (true) {
    idxFitaSelecionada = (idxFitaSelecionada + 1) % QTD_FITAS_FM1;
    while (fitas[idxFitaSelecionada].ehSaida)
      idxFitaSelecionada = 0;

    Fita fitaSaida = fitas[idxFitaSaida];

    Bloco bloco;
    bool leuBloco = BlocoLerViaArquivoBinario(fitaSaida.arquivo, &bloco);
    if (!leuBloco)
      return;

    analise->transferenciasLeitura++;
    bloco.posicaoAtualNoBloco = 0;

    fwrite(&bloco, sizeof(Bloco), 1, fitas[idxFitaSelecionada].arquivo);
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

int FM1ObterFitaDeSaida(Fita *fitas) {
  for (int i = 0; i < QTD_FITAS_FM1; i++)
    if (fitas[i].ehSaida)
      return i;

  return -1;
}

bool FM1FinalizouOProcesso(Fita *fitas) {
  int idxFitaSaida = FM1ObterFitaDeSaida(fitas);
  Bloco bloco;
  int qtdBlocosLidos = 0;
  bool leuBloco;
  do {
    leuBloco = BlocoLerViaArquivoBinario(fitas[idxFitaSaida].arquivo, &bloco);
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