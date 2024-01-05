all: aluno.o compartilhado.o heap.o quickSortExterno.o main.o
				@gcc ./aluno.o ./compartilhado.o ./heap.o ./quickSortExterno.o ./main.o -g -o exe
				@rm *.o

aluno.o: ./src/aluno/aluno.c
				@gcc ./src/aluno/aluno.c -g -c -std=c11 -Wall

compartilhado.o: ./src/compartilhado/compartilhado.c
				@gcc ./src/compartilhado/compartilhado.c -g -c -std=c11 -Wall

heap.o: ./src/heap/heap.c
				@gcc ./src/heap/heap.c -g -c -std=c11 -Wall
		
quickSortExterno.o: ./src/quickSortExterno/quickSortExterno.c
				@gcc ./src/quickSortExterno/quickSortExterno.c -g -c -std=c11 -Wall

intercalacaoBalanceadaFM1.o: ./src/intercalacaoBalanceadaFM1/intercalacaoBalanceadaFM1.c
				@gcc ./src/intercalacaoBalanceadaFM1/intercalacaoBalanceadaFM1.c -g -c -std=c11 -Wall

intercalacaoBalanceada2F.o: ./src/intercalacaoBalanceada2F/intercalacaoBalanceada2F.c
				@gcc ./src/intercalacaoBalanceada2F/intercalacaoBalanceada2F.c -g -c -std=c11 -Wall

main.o: ./src/main/main.c
				@gcc ./src/main/main.c -g -c -std=c11 -Wall