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

// Define as fitas de saida para a estratégia 2F Fitas
void F2DefinirFitasDeSaida(Fita *fitas) {
  for (int i = ceil(QTD_FITAS_2F / 2); i < QTD_FITAS_2F; i++)
    fitas[i].ehSaida = true;
}

// Obtem a fita inicial (pelo tipo [entrada ou saida]) do 2F Fitas
int F2ObterFitaInicial(TipoDeFita tipo) { return tipo == FITA_DE_ENTRADA ? 0 : floor(QTD_FITAS_2F / 2); }

// Obtem a fita final (pelo tipo [entrada ou saida]) do F2 Fitas
int F2ObterFitaFinal(TipoDeFita tipo) { return tipo == FITA_DE_ENTRADA ? floor(QTD_FITAS_2F / 2) - 1 : QTD_FITAS_2F - 1; }

// Obtem a próxima fita pelo tipo
// circular, ou seja, se as fitas de entradas são 1 2 3, esse método retorna 1, 2, 3, 1, 2, 3, 1, 2...
int F2ObterProximaFitaDoTipo(TipoDeFita tipo, int fitaAtual) {
  int fitaInicialDoTipo = F2ObterFitaInicial(tipo);
  int fitaFinalDoTipo = F2ObterFitaFinal(tipo);

  fitaAtual++;
  if (fitaAtual >= fitaFinalDoTipo)
    fitaAtual = fitaInicialDoTipo;

  return fitaAtual;
}

// Função responsável por intercalar as fitas de entrada/saida nas fitas de saida/entrada
// Ou seja, a função principal da intercalação
bool F2JuntarNaFitaDe(Fita *fitas, TipoDeFita tipo, Analise *analise, int *fitaSaida) {
  int fitaInicialDoOutroTipo = F2ObterFitaInicial(tipo == FITA_DE_ENTRADA ? FITA_DE_SAIDA : FITA_DE_ENTRADA);
  int fitaFinalDoOutroTipo = F2ObterFitaFinal(tipo == FITA_DE_ENTRADA ? FITA_DE_SAIDA : FITA_DE_ENTRADA);

  Heap heap = HeapCriar(QTD_FITAS_2F, HEAP_TYPE);
  int fitaAtual = F2ObterFitaInicial(tipo);

  int qtdTotalBlocosFormados = 0;
  while (true) {
    Bloco *blocos = malloc(sizeof(Bloco) * QTD_FITAS_2F);
    int qtdNovoBloco = 0;
    int qtdBlocosLidos = 0;
    // Vai ler o primeiro bloco de cada fita, e junto o primeiro aluno de cada bloco e adicionar no HEAP
    for (int i = fitaInicialDoOutroTipo; i <= fitaFinalDoOutroTipo; i++) {
      bool leuBloco = BlocoLerViaArquivoBinario(fitas[i].arquivo, &blocos[i]);

      if (!leuBloco)
        continue;

      qtdBlocosLidos++;
      qtdNovoBloco += blocos[i].qtdItens;

      Aluno aluno;
      bool leuAluno = AlunoLerViaArquivoBinario(fitas[i].arquivo, &aluno);
      if (!leuAluno)
        continue;

      blocos[i].posicaoAtualNoBloco = 1;
      // Adicionamos o aluno no HEAP, falando que a fita de origem foi a fita da posicão i
      HeapInserirComFitaOrigem(&heap, &aluno, i, analise);
      analise->transferenciasLeitura++;
    }

    // Condição de parada, caso nenhum bloco foi lido, devemos parar a intercalação
    if (qtdBlocosLidos == 0)
      break;

    Bloco novoBloco = BlocoCria(qtdNovoBloco);
    // Criamos um novo bloco a partir da quantidade de items dos blocos da passada atual de cada fita
    while (true) {
      // Loop responsável por remover um item do heap, e ler na fita de origem deste item, um novo item
      Aluno aluno;
      ItemHeap itemRemovido;
      bool removeu = HeapRemove(&heap, &itemRemovido, analise);
      if (!removeu)
        break;

      BlocoInserirAluno(&novoBloco, &itemRemovido.aluno);

      // Caso a posição atual seja a quantidade de itens não tem mais itens a ler
      if (blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco == blocos[itemRemovido.fitaDeOrigem].qtdItens)
        continue;

      AlunoLerViaArquivoBinario(fitas[itemRemovido.fitaDeOrigem].arquivo, &aluno);
      blocos[itemRemovido.fitaDeOrigem].posicaoAtualNoBloco++;
      analise->transferenciasLeitura++;

      HeapInserirComFitaOrigem(&heap, &aluno, itemRemovido.fitaDeOrigem, analise);
    }

    qtdTotalBlocosFormados++;
    // Quando não foi mais possível remover alunos do Heap, temos o Heap todo marcado
    // Então desmarcamos todos e reconstituimos o Heap e então prosseguimos para a próxima fita
    int escritas = BlocoEscreverEmFita(&fitas[fitaAtual], &novoBloco);
    *fitaSaida = fitaAtual;
    analise->transferenciasEscrita += escritas;
    if (DEBUG)
      printf("!Bloco inserido na fita %d com %d itens\n", fitaAtual, novoBloco.qtdItens);
    fitaAtual = F2ObterProximaFitaDoTipo(tipo, fitaAtual);
  }
  // Se leu somente um bloco significa que finalizou o processo
  return qtdTotalBlocosFormados == 1;
}
