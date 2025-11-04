#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ===== Estrutura que representa uma peça do jogo =====
typedef struct {
    char nome; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// ===== Estrutura da fila circular =====
typedef struct {
    Peca elementos[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ===== Estrutura da pilha =====
typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

// ===== Protótipos das funções =====
void inicializarFila(Fila *fila);
void inicializarPilha(Pilha *pilha);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);

void enqueue(Fila *fila, Peca peca);
Peca dequeue(Fila *fila);
void push(Pilha *pilha, Peca peca);
Peca pop(Pilha *pilha);

Peca gerarPeca(int id);
void exibirEstado(Fila *fila, Pilha *pilha);
void trocarTopoFilaComPilha(Fila *fila, Pilha *pilha);
void trocaMultipla(Fila *fila, Pilha *pilha);

int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    srand(time(NULL));
    int idCounter = 0;

    // Inicializa a fila com 5 peças automáticas
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(idCounter++));
    }

    int opcao;
    printf("\n===== TETRIS STACK - GERENCIADOR DE PEÇAS =====\n");

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\nPeça [%c %d] jogada!\n", jogada.nome, jogada.id);
                    // Gera nova peça para manter fila cheia
                    enqueue(&fila, gerarPeca(idCounter++));
                } else {
                    printf("\n⚠️ Fila vazia!\n");
                }
                break;

            case 2: // Reservar peça (fila → pilha)
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("\nPeça [%c %d] movida para a pilha de reserva!\n", reservada.nome, reservada.id);
                    // Reposição automática na fila
                    enqueue(&fila, gerarPeca(idCounter++));
                } else {
                    printf("\n⚠️ Não foi possível reservar: fila vazia ou pilha cheia.\n");
                }
                break;

            case 3: // Usar peça da pilha
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("\nPeça [%c %d] usada e removida da pilha!\n", usada.nome, usada.id);
                } else {
                    printf("\n⚠️ Pilha vazia!\n");
                }
                break;

            case 4: // Trocar frente da fila com topo da pilha
                trocarTopoFilaComPilha(&fila, &pilha);
                break;

            case 5: // Troca múltipla
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o programa... Até logo!\n");
                break;

            default:
                printf("\n⚠️ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ===== Implementações das funções =====

// Inicializa fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

// Inicializa pilha
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Verificações básicas
int filaVazia(Fila *fila) { return fila->quantidade == 0; }
int filaCheia(Fila *fila) { return fila->quantidade == TAM_FILA; }
int pilhaVazia(Pilha *pilha) { return pilha->topo == -1; }
int pilhaCheia(Pilha *pilha) { return pilha->topo == TAM_PILHA - 1; }

// Enfileirar
void enqueue(Fila *fila, Peca peca) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->elementos[fila->tras] = peca;
    fila->quantidade++;
}

// Desenfileirar
Peca dequeue(Fila *fila) {
    Peca removida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->quantidade--;
    return removida;
}

// Empilhar
void push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) return;
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
}

// Desempilhar
Peca pop(Pilha *pilha) {
    Peca removida = pilha->elementos[pilha->topo];
    pilha->topo--;
    return removida;
}

// Gera uma peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Exibe fila e pilha
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n================= ESTADO ATUAL =================\n");
    printf("Fila de peças:\t");
    if (filaVazia(fila))
        printf("[vazia]");
    else {
        int i = fila->frente;
        for (int j = 0; j < fila->quantidade; j++) {
            Peca p = fila->elementos[i];
            printf("[%c %d] ", p.nome, p.id);
            i = (i + 1) % TAM_FILA;
        }
    }

    printf("\nPilha de reserva\t(Topo -> base): ");
    if (pilhaVazia(pilha))
        printf("[vazia]");
    else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->elementos[i].nome, pilha->elementos[i].id);
        }
    }
    printf("\n================================================\n");
}

// Troca o topo da pilha com a frente da fila
void trocarTopoFilaComPilha(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("\n⚠️ Não foi possível trocar: uma das estruturas está vazia.\n");
        return;
    }

    int indiceFrente = fila->frente;
    Peca temp = fila->elementos[indiceFrente];
    fila->elementos[indiceFrente] = pilha->elementos[pilha->topo];
    pilha->elementos[pilha->topo] = temp;

    printf("\nAção: troca simples realizada entre fila e pilha!\n");
}

// Troca múltipla (3 primeiras da fila com 3 da pilha)
void trocaMultipla(Fila *fila, Pilha *pilha) {
    if (fila->quantidade < 3 || pilha->topo < 2) {
        printf("\n⚠️ Troca múltipla não permitida! São necessárias 3 peças em cada estrutura.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indiceFila = (fila->frente + i) % TAM_FILA;
        Peca temp = fila->elementos[indiceFila];
        fila->elementos[indiceFila] = pilha->elementos[pilha->topo - i];
        pilha->elementos[pilha->topo - i] = temp;
    }

    printf("\nAção: troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha!\n");
}
