#include "../lib/intercalacaoBalanceada.h"

// BLOCO

// Função responsável por criar o bloco
Bloco BlocoCria(int qtdMax) {
  Bloco bloco;
  bloco.alunos = malloc(sizeof(Aluno) * qtdMax);
  bloco.qtdItens = 0;
  bloco.qtdMax = qtdMax;
  bloco.posicaoAtualNoBloco = 0;
  return bloco;
}

// Função responsável por inserir um aluno no bloco
bool BlocoInserirAluno(Bloco *bloco, Aluno *a) {
  // Se o bloco está cheio não adiciona o aluno
  if (bloco->qtdMax == bloco->qtdItens)
    return false;

  bloco->alunos[bloco->qtdItens] = *a;
  bloco->qtdItens++;
  return true;
}

// Função para escrever o bloco (e seus alunos independentes) na fita
int BlocoEscreverEmFita(Fita *fita, Bloco *bloco) {
  // Função para escrever um bloco na fita
  fwrite(bloco, sizeof(Bloco), 1, fita->arquivo);
  int escritas = 0;
  for (int i = 0; i < bloco->qtdItens; i++) {
    // Função para escrever aluno na fita
    fwrite(&(bloco->alunos[i]), sizeof(Aluno), 1, fita->arquivo);
    escritas++;
  }
  return escritas;
}

// Função para imprimir bloco no terminal
void BlocoImprimir(Bloco *bloco) {
  printf("\n=================================BLOCO %d================================\n", bloco->qtdItens);
  for (int i = 0; i < bloco->qtdItens; i++) {
    AlunoImprimeCompleto(&bloco->alunos[i]);
    printf("\n");
  }
  printf("\n========================================================================\n");
}

// Função usada para imprimir os metadados de um bloco no terminal
void BlocoImprimeMetadados(Bloco *bloco) {
  printf("BLOCO\n");
  printf("QTD Itens: %d\n", bloco->qtdItens);
}

// Função para ler um bloco de um arquivo binário
bool BlocoLerViaArquivoBinario(FILE *arquivo, Bloco *bloco) { return fread(bloco, sizeof(Bloco), 1, arquivo) == 1; }

// FITA

// Função responsável por criar a fita
Fita FitaCria(FILE *arquivo, bool ehSaida) {
  Fita f;
  f.arquivo = arquivo;
  f.ehSaida = ehSaida;
  return f;
}

// Função responsável por obter o caminho relativo da fita (caminho do arquivo fisico que irá representa a fita)
char *FitaObterCaminhoPelaPosicao(int p, EstrategiaDeIntercalacao estrategia) {
  char *caminhoArquivo = malloc(sizeof(char) * 52);

  // Arquivo fica em ./fitas/FM1/fita_<i>.bin
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

// Função para fechar os arquivos que simulam as fitas
void FitaFecharArquivos(Fita *fitas, EstrategiaDeIntercalacao estrategia) {
  for (int i = 0; i < FitaObterQuantidadeDeFitas(estrategia); i++)
    fclose(fitas[i].arquivo);
}

// Função para imprimir uma fita
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

// Função de geração dos blocos
Fita *FitaGerarBlocos(int qtdLinhas, EstrategiaDeIntercalacao estrategia, Analise *analise, int *blocosGerados) {
  FILE *arquivoProvao = fopen("./arquivos/output.txt", "r");
  if (arquivoProvao == NULL) {
    printf("Erro(output.txt): ");
    return NULL;
  }
  // Cria as fitas (quantidade definida pelas constantes)
  Fita *fitas = FitaGerarFitas(FitaObterQuantidadeDeFitas(estrategia), estrategia);
  if (estrategia == FM1)
    FM1DefinirFitaSaida(fitas); // Define a fita de saida do F+1 fitas (ultima fita)
  else
    // Define as fitas de saidas do 2F fitas (a partir da posição do meio, arredondada pra cima em caso de número par)
    F2DefinirFitasDeSaida(fitas);

  Heap heap = HeapCriar(FitaObterQuantidadeDeFitas(estrategia), HEAP_TYPE);

  int qtdAlunosLidos = 0;
  while (!HeapCheio(&heap) && qtdAlunosLidos < qtdLinhas) {
    // Loop responsável por ler os alunos do arquivo e preencher o Heap
    Aluno a = AlunoLer(arquivoProvao);
    analise->transferenciasLeitura++;

    qtdAlunosLidos++;
    HeapInserir(&heap, &a, analise);
  }

  int fitaSelecionada = 0;
  while (true) {
    // Loop responsável por criar o bloco e ler os alunos até o heap ficar com todos seus elementos marcados
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
        // Caso onde devemos parar de ler novos alunos
        // Final do arquivo ou já ultrapassaoms a quantidade de alunos a ser lidos

        while (HeapRemove(&heap, &itemRemovido, analise)) // Tira todos os alunos do heap e insere no bloco atual
          BlocoInserirAluno(&bloco, &itemRemovido.aluno); //

        if (bloco.qtdItens > 0) { // Caso o bloco seja valido, insere o bloco na fita
          (*blocosGerados)++;
          int escritas = BlocoEscreverEmFita(&fitas[fitaSelecionada], &bloco);
          analise->transferenciasEscrita += escritas;
          if (DEBUG)
            printf("!Bloco inserido na fita %d com %d itens\n", fitaSelecionada, bloco.qtdItens);
          fitaSelecionada = (fitaSelecionada + 1) % FitaObterIndexPrimeiraFitaDeEntradas(fitas, estrategia);
        }

        // Caso o heap ainda possua itens marcados
        // Desmarcamos, reconstituimos o heap e criamos um novo bloco e inserimos esses itens reconstituidos
        HeapDesmarcarTodosEReconstituir(&heap, analise);

        Bloco blocoDosMarcadosRestantes = BlocoCria(qtdLinhas);
        while (HeapRemove(&heap, &itemRemovido, analise))
          BlocoInserirAluno(&blocoDosMarcadosRestantes, &itemRemovido.aluno);

        // Caso o bloco dos itens "marcados" (reconstituidos) seja valido, escrevemos na proxima fita
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

      // Caso devemos continuar lendo
      // Lemos o aluno
      Aluno a = AlunoLer(arquivoProvao);
      analise->transferenciasLeitura++;
      qtdAlunosLidos++;
      // E inserimos-o no grupo
      HeapInserir(&heap, &a, analise);

      // Se não conseguiu remover devemos escrevemos o bloco na fita desmarcamos todos e reconstituimos o heap
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

// Função responsável por resetar a posição dos arquivos
void FitaResetarArquivos(Fita *fitas, EstrategiaDeIntercalacao estrategia) {
  for (int i = 0; i < FitaObterQuantidadeDeFitas(estrategia); i++)
    fseek(fitas[i].arquivo, 0, 0);
}

// Função responsável por regerar os arquivos pelo tipo do arquivo
void FitaRegerarFitas(Fita *fitas, TipoDeFita tipo, EstrategiaDeIntercalacao estrategia) {
  for (int i = 0; i < FitaObterQuantidadeDeFitas(estrategia); i++) {
    // Caso seja solicitado regerar os arquivo de saida e o arquivo for de entrada ou ao contrario devemos ignorar e ir para a
    // próxima fita
    if ((tipo == FITA_DE_SAIDA && !fitas[i].ehSaida) || (tipo == FITA_DE_ENTRADA && fitas[i].ehSaida))
      continue;

    char *caminhoArquivo = FitaObterCaminhoPelaPosicao(i, estrategia);
    fclose(fitas[i].arquivo);

    FILE *arquivo = fopen(caminhoArquivo, "w+b");
    fitas[i].arquivo = arquivo;
    free(caminhoArquivo);
  }
}

// Função responsável por gerar as fitas, gera também as pastas e cada arquivo
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
    // Gera a quantidade de fitas pelo parâmetro qtd
    char *caminhoArquivo = FitaObterCaminhoPelaPosicao(i, estrategia);
    FILE *arquivo = fopen(caminhoArquivo, "w+b");
    fitas[i] = FitaCria(arquivo, false);

    for (int i = 0; i < 52; i++)
      caminhoArquivo[i] = '\0';

    free(caminhoArquivo);
  }

  return fitas;
}

// Função responsável por obter o index da primeira fita de entrada
int FitaObterIndexPrimeiraFitaDeEntradas(Fita *f, EstrategiaDeIntercalacao estrategia) {
  int c = 0;
  for (int i = 0; i < FitaObterQuantidadeDeFitas(estrategia); i++) {
    if (!f[i].ehSaida)
      c++;
  }
  return c - 1;
}

// Função para obter a quantidade de fitas
int FitaObterQuantidadeDeFitas(EstrategiaDeIntercalacao estrategia) { return estrategia == FM1 ? QTD_FITAS_FM1 : QTD_FITAS_2F; }

// INTERCALACAO

// Função principal do programa
void IntercalacaoBalanceada(EstrategiaDeIntercalacao estrategia, int linhasALer, Analise *analise) {
  struct timespec inicio, fim;

  // Parte de geração dos blocos
  clock_gettime(1, &inicio);
  int blocosGerados = 0;
  Fita *fitas = FitaGerarBlocos(linhasALer, estrategia, analise, &blocosGerados);
  if (fitas == NULL) {
    printf("Ocorreu um erro inesperado!");
    exit(1);
  }

  if (blocosGerados <= 1) {
    // Caso seja gerado somente um bloco, foi gerado um bloco ordenado, sendo assim, não necessário passar para a fase de
    // intercalação
    copiaArquivoBin(fitas[0].arquivo, linhasALer);

    FitaFecharArquivos(fitas, estrategia);
    clock_gettime(1, &fim);
    AnaliseDefinirTempoPeloInicioEFim(analise, inicio, fim);
    return;
  }
  // Fim da parte de geração dos blocos

  bool finalizou = false;
  int fitaSaida;
  if (estrategia == FM1) {
    // Fase de intercalação F+1 fitas
    while (true) {

      FitaResetarArquivos(fitas, estrategia);
      bool finalizou = FM1JuntarNaFitaDeSaida(fitas, analise, &fitaSaida);
      FitaRegerarFitas(fitas, FITA_DE_ENTRADA, FM1);

      if (finalizou) {
        // Fim da fase de intercalação F+1 fitas
        break;
      }

      FitaResetarArquivos(fitas, estrategia);
      FM1EspalharBlocosDaSaida(fitas, analise);
      FitaRegerarFitas(fitas, FITA_DE_SAIDA, FM1);
    }
  } else {
    // Fase de intercalação 2F fitas
    while (true) {
      FitaResetarArquivos(fitas, estrategia);
      finalizou = F2JuntarNaFitaDe(fitas, FITA_DE_SAIDA, analise, &fitaSaida);
      FitaRegerarFitas(fitas, FITA_DE_ENTRADA, F2);

      if (finalizou) {
        // Fim da fase de intercalação 2F fitas
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

  // Após a intercalação copiamos o arquivo da fita onde obtivemos o bloco gerado final ordenado
  // e copiamos para o arquivo /arquivos/output.txt
  copiaArquivoBin(fitas[fitaSaida].arquivo, linhasALer);

  // Fechamos os arquivos das fitas e finalizamos o programa
  FitaFecharArquivos(fitas, estrategia);
  clock_gettime(1, &fim);
  AnaliseDefinirTempoPeloInicioEFim(analise, inicio, fim);
}
