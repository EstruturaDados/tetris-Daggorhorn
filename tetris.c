#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

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

// Estrutura da pilha (reserva)
typedef struct {
    Peca elementos[TAMANHO_PILHA];
    int topo;
} Pilha;

// Variável para controlar o ID das peças
int contadorID = 0;

// Função para gerar automaticamente uma nova peça
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;

    nova.nome = tipos[rand() % 4]; // Escolhe tipo aleatório
    nova.id = contadorID++; // Atribui ID único

    return nova;
}

// Inicialização da fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;

    // Preenche inicialmente com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        f->elementos[f->fim] = gerarPeca();
        f->fim = (f->fim + 1) % TAMANHO_FILA;
        f->quantidade++;
    }
}

// Inicialização da pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verificações auxiliares
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAMANHO_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Remove uma peça da fila (dequeue) 
Peca removerDaFila(Fila *f) {
    Peca removida = f->elementos[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;
    return removida;
}

// Adiciona nova peça na fila (enqueue)
void adicionarNovaNaFila(Fila *f) {
    f->elementos[f->fim] = gerarPeca();
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->quantidade++;
}

// Jogar peça da frente da fila
void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf("\nFila vazia!\n");
        return;
    }

    Peca jogada = removerDaFila(f);
    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);

    adicionarNovaNaFila(f);
}

// Reservar peça (fila -> pilha)
void reservarPeca(Fila *f, Pilha *p) {
    if (pilhaCheia(p)) {
        printf("\nPilha de reserva cheia!\n");
        return;
    }

    Peca reservada = removerDaFila(f);

    p->topo++;
    p->elementos[p->topo] = reservada;

    printf("\nPeça reservada: [%c %d]\n", reservada.nome, reservada.id);

    adicionarNovaNaFila(f);
}

// Usar peça reservada (pilha -> jogo)
void usarReserva(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nPilha vazia!\n");
        return;
    }

    Peca usada = p->elementos[p->topo];
    p->topo--;

    printf("\nPeça usada da reserva: [%c %d]\n", usada.nome, usada.id);
}

// Exibir estado atual da fila e pilha
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n==========================================\n");
    printf("Estado atual:\n\n");

    printf("Fila de peças:\n");
    int indice = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->elementos[indice].nome,
                            f->elementos[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }

    printf("\n\nPilha de reserva (Topo -> Base):\n");
    if (pilhaVazia(p)) {
        printf("[Vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->elementos[i].nome,
                                p->elementos[i].id);
        }
    }

    printf("\n==========================================\n");
}

// Menu de opções
void menu() {
    printf("\nOpções de Ação:\n");
    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("0 - Sair\n");
    printf("Opção: ");
}

// Função principal
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;

    do {
        exibirEstado(&fila, &pilha);
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarReserva(&pilha);
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