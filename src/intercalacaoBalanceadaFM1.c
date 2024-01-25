#include "../lib/heap.h"
#include "../lib/intercalacaoBalanceada.h"

// Função responsável por definir as fitas de saida do F + 1 fitas
void FM1DefinirFitaSaida(Fita *fitas) { fitas[QTD_FITAS_FM1 - 1].ehSaida = true; }

// Função responsável por intercalar as fitas de entrada na fita de saida
bool FM1JuntarNaFitaDeSaida(Fita *fitas, Analise *analise, int *fitaSaidaIndex) {
  Heap heap = HeapCriar(QTD_FITAS_FM1, HEAP_TYPE);
  Bloco *blocos = malloc(sizeof(Bloco) * QTD_FITAS_FM1);
  // Controlamos os blocos escritos pois, quando escrevemos somente um bloco na fita de saida, o processo foi finalizado, e temos
  // o arquivo ordenado
  int qtdBlocosEscritos = 0;
  do {
    int qtdNovoBloco = 0;
    int qtdBlocosLidos = 0;
    // Loop responsável por ler o primeiro bloco de cada fita e o primeiro aluno para inserir no Heap para podermos intercalar na
    // fita de saida
    for (int i = 0; i < QTD_FITAS_FM1 - 2; i++) {
      int leuBloco = BlocoLerViaArquivoBinario(fitas[i].arquivo, &blocos[i]);
      if (!leuBloco)
        continue;

      blocos[i].posicaoAtualNoBloco = 0;
      qtdNovoBloco += blocos[i].qtdItens;
      qtdBlocosLidos++;

      Aluno aluno;
      bool leuAluno = AlunoLerViaArquivoBinario(fitas[i].arquivo, &aluno);
      if (!leuAluno)
        continue;

      analise->transferenciasLeitura++;
      blocos[i].posicaoAtualNoBloco++;
      // Inserimos o aluno lido no Heap, e adicionamos um a posição atual do bloco
      HeapInserirComFitaOrigem(&heap, &aluno, i, analise);
    }
    // Caso nenhum bloco seja lido, significa que não tem mais itens a ser intercalados
    if (qtdBlocosLidos == 0)
      break;

    bool removeu = true;
    // Criamos um novo bloco com o tamanho a partir dos blocos lidos
    Bloco novoBloco = BlocoCria(qtdNovoBloco);
    ItemHeap itemRemovido;
    do {
      removeu = HeapRemove(&heap, &itemRemovido, analise);
      if (removeu) {
        // Loop responsável por remover um item do heap, e ler na fita de origem deste item, um novo item
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
    // Quando não foi mais possível remover alunos do Heap, temos o Heap todo marcado
    // Então desmarcamos todos e reconstituimos o Heap e então prosseguimos para a próxima fita
    int escritas = BlocoEscreverEmFita(&fitas[saida], &novoBloco);
    analise->transferenciasEscrita += escritas;
    qtdBlocosEscritos++;
    *fitaSaidaIndex = saida;
  } while (true);
  // Caso somente um bloco seja escrito finalizou o processo
  return qtdBlocosEscritos == 1;
}

// Função responsável por espalhar os blocos das fitas de entrada na fita de saida
void FM1EspalharBlocosDaSaida(Fita *fitas, Analise *analise) {
  int idxFitaSaida = FM1ObterFitaDeSaida(fitas);

  int idxFitaSelecionada = -1;
  while (true) {
    // Loop responsável por ler cada bloco da fita de saida e jogar em cada fita de entrada
    // nas fitas 1, 2, 3, ..., n, 1, 2, 3, ..., 1, 2, 3 ...
    idxFitaSelecionada = (idxFitaSelecionada + 1) % QTD_FITAS_FM1;
    if (fitas[idxFitaSelecionada].ehSaida)
      idxFitaSelecionada = 0;

    Fita fitaSaida = fitas[idxFitaSaida];

    Bloco bloco;
    bool leuBloco = BlocoLerViaArquivoBinario(fitaSaida.arquivo, &bloco);
    if (!leuBloco)
      return;

    analise->transferenciasLeitura++;
    bloco.posicaoAtualNoBloco = 0;

    // Le o bloco do arquivo de saida e escreve no arquivo de entrada selecionado
    fwrite(&bloco, sizeof(Bloco), 1, fitas[idxFitaSelecionada].arquivo);
    analise->transferenciasEscrita++;

    if (DEBUG)
      printf("BLOCO de saida (%d): %d\n", idxFitaSelecionada, bloco.qtdItens);
    Aluno aluno;

    // Loop responsável por copiar os alunos do bloco da fita de saida no bloco da fita de entrada
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

// Função responsável por obter a fita de saida do heap
int FM1ObterFitaDeSaida(Fita *fitas) {
  for (int i = 0; i < QTD_FITAS_FM1; i++)
    if (fitas[i].ehSaida)
      return i;

  return -1;
}

// Função responsável das por verificar se finalizou o processo, ou seja, tem somente um bloco
bool FM1FinalizouOProcesso(Fita *fitas) {
  int idxFitaSaida = FM1ObterFitaDeSaida(fitas);
  Bloco bloco;
  int qtdBlocosLidos = 0;
  bool leuBloco;
  do {
    // Loop responsável por ler um bloco do arquivo binário e
    leuBloco = BlocoLerViaArquivoBinario(fitas[idxFitaSaida].arquivo, &bloco);
    if (!leuBloco)
      continue;

    qtdBlocosLidos++;

    // Se leu mais que um bloco, devemos continuar o processo, ou seja, não finalizou o processo
    if (qtdBlocosLidos > 1)
      return false;
  } while (leuBloco);

  // Se leu exatamente um bloco finalizou o processo
  return qtdBlocosLidos == 1;
}