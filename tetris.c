#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5   // Tamanho fixo da fila em 5 peças

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura que representa a fila circular
typedef struct {
    Peca elementos[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Variável para controlar o ID das peças
int contadorID = 0; 

// Função para gerar automaticamente uma nova peça
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    novaPeca.nome = tipos[rand() % 4]; // Escolhe tipo aleatório
    novaPeca.id = contadorID++;        // Atribui ID único
    
    return novaPeca;
}

// Inicializa a fila
void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->quantidade = 0;
}

// Verifica se a fila está cheia ou vazia
int filaCheia(Fila *fila) {
    return fila->quantidade == TAMANHO_FILA;
}

int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

// Insere uma peça na fila (enqueue)
void enqueue(Fila *fila, Peca novaPeca) {
    if (filaCheia(fila)) {
        printf("\n A fila está cheia! Não é possível inserir nova peça.\n");
        return;
    }

    fila->elementos[fila->fim] = novaPeca;
    fila->fim = (fila->fim + 1) % TAMANHO_FILA; // Movimento circular
    fila->quantidade++;
}

// Remove uma peça da fila (dequeue)
void dequeue(Fila *fila) {
    if (filaVazia(fila)) {
        printf("\n A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }

    Peca removida = fila->elementos[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAMANHO_FILA;
    fila->quantidade--;

    printf("\n Peça jogada: [%c %d]\n", removida.nome, removida.id);
}

// Exibe o estado atual da fila
void exibirFila(Fila *fila) {
    printf("\n====================================\n");
    printf("Fila de peças:\n");

    if (filaVazia(fila)) {
        printf("[Fila vazia]\n");
    } else {
        int indice = fila->inicio;
        for (int i = 0; i < fila->quantidade; i++) {
            printf("[%c %d] ", fila->elementos[indice].nome,
                                fila->elementos[indice].id);
            indice = (indice + 1) % TAMANHO_FILA;
        }
        printf("\n");
    }

    printf("====================================\n");
}


// Menu principal
void menu() {
    printf("\nOpções de ação:\n");
    printf("1 - Jogar peça (dequeue)\n");
    printf("2 - Inserir nova peça (enqueue)\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

//  Função principal
int main() {
    srand(time(NULL)); // Inicializa aleatoriedade

    Fila fila;
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;

    do {
        exibirFila(&fila);
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                dequeue(&fila);
                break;
            case 2:
                enqueue(&fila, gerarPeca());
                break;
            case 0:
                printf("\nObrigado por jogar! Encerrando programa...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}