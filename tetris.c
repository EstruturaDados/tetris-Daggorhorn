#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  
    int id;     // Identificador único
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
int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAMANHO_PILHA - 1; }

Peca removerDaFila(Fila *f) {
    Peca removida = f->elementos[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;
    return removida;
}

void adicionarNovaNaFila(Fila *f) {
    f->elementos[f->fim] = gerarPeca();
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->quantidade++;
}

// Jogar peça (remove da fila e gera nova)
void jogarPeca(Fila *f) {
    Peca jogada = removerDaFila(f);
    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
    adicionarNovaNaFila(f);
}

// Reservar peça (fila -> pilha)
void reservarPeca(Fila *f, Pilha *p) {
    if (pilhaCheia(p)) {
        printf("\nPilha cheia! Não é possível reservar.\n");
        return;
    }

    Peca movida = removerDaFila(f);
    p->elementos[++p->topo] = movida;

    printf("\nPeça enviada para reserva: [%c %d]\n", movida.nome, movida.id);
    adicionarNovaNaFila(f);
}

// Usar peça reservada 
void usarReserva(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nPilha vazia!\n");
        return;
    }

    Peca usada = p->elementos[p->topo--];
    printf("\nPeça usada da reserva: [%c %d]\n", usada.nome, usada.id);
}

// Troca simples entre frente da fila e topo da pilha
void trocarAtual(Fila *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nPilha vazia! Não é possível trocar.\n");
        return;
    }

    Peca temp = f->elementos[f->inicio];
    f->elementos[f->inicio] = p->elementos[p->topo];
    p->elementos[p->topo] = temp;

    printf("\nTroca realizada entre frente da fila e topo da pilha.\n");
}

// Troca múltipla entre os 3 primeiros da fila e os 3 da pilha
void trocaMultipla(Fila *f, Pilha *p) {
    if (p->topo < 2 || f->quantidade < 3) {
        printf("\nNão há peças suficientes para troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indiceFila = (f->inicio + i) % TAMANHO_FILA;
        int indicePilha = p->topo - i;

        Peca temp = f->elementos[indiceFila];
        f->elementos[indiceFila] = p->elementos[indicePilha];
        p->elementos[indicePilha] = temp;
    }

    printf("\nTroca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// Exibir estado atual da fila e pilha
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n==============================================\n");
    printf("Estado atual:\n\n");

    printf("Fila de peças: ");
    int indice = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->elementos[indice].nome,
                            f->elementos[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[Vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->elementos[i].nome,
                                p->elementos[i].id);
        }
    }

    printf("\n==============================================\n");
}

// Menu de opções
void menu() {
    printf("\nOpções disponíveis:\n");
    printf("1 - Jogar peça da frente da fila\n");
    printf("2 - Enviar peça da fila para a pilha\n");
    printf("3 - Usar peça da pilha\n");
    printf("4 - Trocar peça da frente com topo da pilha\n");
    printf("5 - Trocar 3 primeiras da fila com 3 da pilha\n");
    printf("0 - Sair\n");
    printf("Opção escolhida: ");
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
            case 1: jogarPeca(&fila);
            break;
            case 2: reservarPeca(&fila, &pilha);
            break;
            case 3: usarReserva(&pilha);
            break;
            case 4: trocarAtual(&fila, &pilha);
            break;
            case 5: trocaMultipla(&fila, &pilha);
            break;
            case 0: printf("\nObrigado por jogar! Encerrando programa...\n");
            break;
            default: printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}