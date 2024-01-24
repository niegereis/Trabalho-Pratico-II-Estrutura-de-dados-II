#include "../lib/intercalacaoBalanceada.h"

// BLOCO

Bloco BlocoCria(int qtdMax) {
  Bloco bloco;
  bloco.alunos = malloc(sizeof(Aluno) * qtdMax);
  bloco.qtdItens = 0;
  bloco.qtdMax = qtdMax;
  bloco.posicaoAtualNoBloco = 0;
  return bloco;
}

bool BlocoInserirAluno(Bloco *bloco, Aluno *a) {
  if (bloco->qtdMax == bloco->qtdItens)
    return false;

  bloco->alunos[bloco->qtdItens] = *a;
  bloco->qtdItens++;
  return true;
}

int BlocoEscreverEmFita(Fita *fita, Bloco *bloco) {
  fwrite(bloco, sizeof(Bloco), 1, fita->arquivo);
  int escritas = 0;
  for (int i = 0; i < bloco->qtdItens; i++) {
    fwrite(&(bloco->alunos[i]), sizeof(Aluno), 1, fita->arquivo);
    escritas++;
  }
  return escritas;
}

void BlocoImprimir(Bloco *bloco) {
  printf("\n=================================BLOCO %d================================\n", bloco->qtdItens);
  for (int i = 0; i < bloco->qtdItens; i++) {
    AlunoImprimeCompleto(&bloco->alunos[i]);
    printf("\n");
  }
  printf("\n========================================================================\n");
}

void BlocoImprimeMetadados(Bloco *bloco) {
  printf("BLOCO\n");
  printf("QTD Itens: %d\n", bloco->qtdItens);
}

bool BlocoLerViaArquivoBinario(FILE *arquivo, Bloco *bloco) { return fread(bloco, sizeof(Bloco), 1, arquivo) == 1; }

// FITA

Fita FitaCria(FILE *arquivo, bool ehSaida) {
  Fita f;
  f.arquivo = arquivo;
  f.ehSaida = ehSaida;
  return f;
}

char *FitaObterCaminhoPelaPosicao(int p, EstrategiaDeIntercalacao estrategia) {
  char *caminhoArquivo = malloc(sizeof(char) * 52);

  if (estrategia == FM1)
    strncpy(caminhoArquivo, "./fitas/FM1/fita_", 44);
  else
    strncpy(caminhoArquivo, "./fitas/2F/fita_", 43);

  // converte numero para string
  char stringNumero[4];
  sprintf(stringNumero, "%d", p);

  // concatena a palavra fita_ com o valor textual da variável i
  strncat(caminhoArquivo, stringNumero, p < 9 ? 1 : 2);

  // concatena .txt a palavra fita_<i>
  strncat(caminhoArquivo, ".bin", 5);
  return caminhoArquivo;
}

void FitaFecharArquivos(Fita *fitas, EstrategiaDeIntercalacao estrategia) {
  for (int i = 0; i < FitaObterQuantidadeDeFitas(estrategia); i++)
    fclose(fitas[i].arquivo);
}

void FitaImprime(Fita *fita) {
  fseek(fita->arquivo, 0, 0);
  Bloco bloco;
  bool leu = BlocoLerViaArquivoBinario(fita->arquivo, &bloco);
  if (!leu) {
    perror("Não foi possivel ler o bloco. Erro: ");
    return;
  }
  BlocoImprimeMetadados(&bloco);
  int i = 0;
  while (bloco.posicaoAtualNoBloco < bloco.qtdItens) {
    Aluno aluno;
    AlunoLerViaArquivoBinario(fita->arquivo, &aluno);
    printf("(%d): ", i++);
    AlunoImprime(&aluno);
    printf("\n");
    bloco.posicaoAtualNoBloco++;
  }
}

Fita *FitaGerarBlocos(int qtdLinhas, EstrategiaDeIntercalacao estrategia, Analise *analise, int *blocosGerados) {
  FILE *arquivoProvao = fopen("./arquivos/output.txt", "r");
  if (arquivoProvao == NULL) {
    printf("Erro(output.txt): ");
    return NULL;
  }
  Fita *fitas = FitaGerarFitas(FitaObterQuantidadeDeFitas(estrategia), estrategia);
  if (estrategia == FM1)
    FM1DefinirFitaSaida(fitas);
  else
    F2DefinirFitasDeSaida(fitas);

  Heap heap = HeapCriar(FitaObterQuantidadeDeFitas(estrategia), HEAP_TYPE);

  int qtdAlunosLidos = 0;
  while (!HeapCheio(&heap) && qtdAlunosLidos < qtdLinhas) {
    Aluno a = AlunoLer(arquivoProvao);
    analise->transferenciasLeitura++;

    qtdAlunosLidos++;
    HeapInserir(&heap, &a, analise);
  }

  int fitaSelecionada = 0;
  while (true) {
    Bloco bloco = BlocoCria(qtdLinhas);
    bool removeu;
    do {
      ItemHeap itemRemovido;
      removeu = HeapRemove(&heap, &itemRemovido, analise);
      if (removeu) {
        BlocoInserirAluno(&bloco, &itemRemovido.aluno);
      } else if (heap.ultimoRemovido != NULL) {
        break;
      }

      if (feof(arquivoProvao) || qtdAlunosLidos >= qtdLinhas) {
        while (HeapRemove(&heap, &itemRemovido, analise))
          BlocoInserirAluno(&bloco, &itemRemovido.aluno);

        if (bloco.qtdItens > 0) {
          (*blocosGerados)++;
          int escritas = BlocoEscreverEmFita(&fitas[fitaSelecionada], &bloco);
          analise->transferenciasEscrita += escritas;
          if (DEBUG)
            printf("!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, bloco.qtdItens);
          fitaSelecionada = (fitaSelecionada + 1) % FitaObterIndexPrimeiraFitaDeEntradas(fitas, estrategia);
        }

        HeapRemoverDesmarcados(&heap);
        HeapDesmarcarTodosEReconstituir(&heap, analise);

        Bloco blocoDosMarcadosRestantes = BlocoCria(10000);
        while (HeapRemove(&heap, &itemRemovido, analise))
          BlocoInserirAluno(&blocoDosMarcadosRestantes, &itemRemovido.aluno);
        if (blocoDosMarcadosRestantes.qtdItens > 0) {
          (*blocosGerados)++;
          int escritas = BlocoEscreverEmFita(&fitas[fitaSelecionada], &blocoDosMarcadosRestantes);
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

    int escritas = BlocoEscreverEmFita(&fitas[fitaSelecionada], &bloco);
    if (bloco.qtdItens > 0)
      (*blocosGerados)++;

    analise->transferenciasEscrita += escritas;
    if (DEBUG)
      printf("!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, bloco.qtdItens);
    fitaSelecionada = (fitaSelecionada + 1) % FitaObterIndexPrimeiraFitaDeEntradas(fitas, estrategia);

    HeapDesmarcarTodosEReconstituir(&heap, analise);
  }

  return fitas;
}

void FitaResetarArquivos(Fita *fitas, EstrategiaDeIntercalacao estrategia) {
  for (int i = 0; i < FitaObterQuantidadeDeFitas(estrategia); i++)
    fseek(fitas[i].arquivo, 0, 0);
}

void FitaRegerarFitas(Fita *fitas, TipoDeFita tipo, EstrategiaDeIntercalacao estrategia) {
  for (int i = 0; i < FitaObterQuantidadeDeFitas(estrategia); i++) {
    if ((tipo == FITA_DE_SAIDA && !fitas[i].ehSaida) || (tipo == FITA_DE_ENTRADA && fitas[i].ehSaida))
      continue;

    char *caminhoArquivo = FitaObterCaminhoPelaPosicao(i, estrategia);
    fclose(fitas[i].arquivo);

    FILE *arquivo = fopen(caminhoArquivo, "w+b");
    fitas[i].arquivo = arquivo;
    free(caminhoArquivo);
  }
}

Fita *FitaGerarFitas(int qtd, EstrategiaDeIntercalacao estrategia) {
  if (!opendir("./fitas/"))
    mkdir("./fitas/", 0777);

  if (estrategia == FM1) {
    if (!opendir("./fitas/FM1/"))
      mkdir("./fitas/FM1/", 0777);
  } else {
    if (!opendir("./fitas/2F/")) {
      mkdir("./fitas/2F/", 0777);
    }
  }
  Fita *fitas = malloc(sizeof(Fita) * qtd);

  for (int i = 0; i < qtd; i++) {

    char *caminhoArquivo = FitaObterCaminhoPelaPosicao(i, estrategia);
    FILE *arquivo = fopen(caminhoArquivo, "w+b");
    fitas[i] = FitaCria(arquivo, false);

    for (int i = 0; i < 52; i++)
      caminhoArquivo[i] = '\0';

    free(caminhoArquivo);
  }

  return fitas;
}

int FitaObterIndexPrimeiraFitaDeEntradas(Fita *f, EstrategiaDeIntercalacao estrategia) {
  int c = 0;
  for (int i = 0; i < FitaObterQuantidadeDeFitas(estrategia); i++) {
    if (!f[i].ehSaida)
      c++;
  }
  return c - 1;
}

int FitaObterQuantidadeDeFitas(EstrategiaDeIntercalacao estrategia) { return estrategia == FM1 ? QTD_FITAS_FM1 : QTD_FITAS_2F; }

int FitaObterQuantidadeItemsDoHeap(EstrategiaDeIntercalacao estrategia) {
  return estrategia == FM1 ? QTD_FITAS_FM1 - 1 : QTD_FITAS_2F / 2;
}

// void FitaConverterParaArquivoOutput(Fita *fita) {
//   fseek(fita->arquivo, sizeof(Bloco), 0);
//   FILE *output = fopen("./arquivos/output.txt", "w+");
//   bool leu = true;
//   while (leu) {
//     Aluno aluno;
//     int qtd = fread(&aluno, sizeof(Aluno), 1, fita->arquivo);
//     leu = qtd == 1;
//     AlunoEscreveSeparadosPorNovaLinha(output, &aluno);
//   }
//   fclose(output);
// }

// INTERCALACAO

void IntercalacaoBalanceada(EstrategiaDeIntercalacao estrategia, int linhasALer, Analise *analise) {
  struct timespec inicio, fim;

  clock_gettime(1, &inicio);
  int blocosGerados = 0;
  Fita *fitas = FitaGerarBlocos(linhasALer, estrategia, analise, &blocosGerados);
  if (fitas == NULL) {
    printf("Ocorreu um erro inesperado!");
    exit(1);
  }

  printf("Blocos gerados: %d\n", blocosGerados);

  if (blocosGerados <= 1) {
    copiaArquivoBin(fitas[0].arquivo, linhasALer);
    return;
  }

  bool finalizou = false;
  int fitaSaida;
  if (estrategia == FM1) {
    while (true) {

      FitaResetarArquivos(fitas, estrategia);
      bool x = FM1JuntarNaFitaDeSaida(fitas, analise, &fitaSaida);
      FitaRegerarFitas(fitas, FITA_DE_ENTRADA, FM1);

      if (x) {
        break;
      }

      FitaResetarArquivos(fitas, estrategia);
      FM1EspalharBlocosDaSaida(fitas, analise);
      FitaRegerarFitas(fitas, FITA_DE_SAIDA, FM1);
    }
  } else {
    while (true) {
      FitaResetarArquivos(fitas, estrategia);
      finalizou = F2JuntarNaFitaDe(fitas, FITA_DE_SAIDA, analise, &fitaSaida);
      FitaRegerarFitas(fitas, FITA_DE_ENTRADA, F2);

      if (finalizou) {
        break;
      }

      FitaResetarArquivos(fitas, estrategia);
      finalizou = F2JuntarNaFitaDe(fitas, FITA_DE_ENTRADA, analise, &fitaSaida);
      FitaRegerarFitas(fitas, FITA_DE_SAIDA, F2);

      if (finalizou) {
        break;
      }
    }
  }

  // FitaConverterParaArquivoOutput(&fitas[fitaSaida]);
  copiaArquivoBin(fitas[fitaSaida].arquivo, linhasALer);

  FitaFecharArquivos(fitas, estrategia);
  clock_gettime(1, &fim);
  AnaliseDefinirTempoPeloInicioEFim(analise, inicio, fim);
}
