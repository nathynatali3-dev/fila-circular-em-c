#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 5
#define TAM_NOME 50

typedef struct {
    int senha;
    char nome[TAM_NOME];
} Pessoa;

typedef struct {
    Pessoa dados[MAX];
    int inicio;      /* indice da proxima pessoa a ser chamada */
    int fim;         /* indice onde a proxima pessoa sera inserida */
    int quantidade;  /* total de pessoas na fila */
} Fila;

void inicializar(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

bool estaVazia(Fila *f) { return f->quantidade == 0; }
bool estaCheia(Fila *f) { return f->quantidade == MAX; }

void mostrarEstado(Fila *f) {
    printf("[ESTADO] inicio = %d | fim = %d | quantidade = %d/%d",
           f->inicio, f->fim, f->quantidade, MAX);
    if (estaCheia(f))      printf(" -> FILA CHEIA");
    else if (estaVazia(f)) printf(" -> FILA VAZIA");
    printf("\n");
}

bool inserir(Fila *f, Pessoa p) {
    if (estaCheia(f)) {
        printf("OVERFLOW: fila cheia, insercao nao permitida!\n");
        return false;
    }
    f->dados[f->fim] = p;
    printf("Senha %d (%s) inserida na posicao %d.\n", p.senha, p.nome, f->fim);
    f->fim = (f->fim + 1) % MAX;   /* movimento circular */
    f->quantidade++;
    return true;
}

bool chamarProxima(Fila *f, Pessoa *p) {
    if (estaVazia(f)) {
        printf("UNDERFLOW: fila vazia, nao ha ninguem para chamar!\n");
        return false;
    }
    *p = f->dados[f->inicio];
    printf("Chamando senha %d - %s (posicao %d liberada).\n",
           p->senha, p->nome, f->inicio);
    f->inicio = (f->inicio + 1) % MAX;   /* movimento circular */
    f->quantidade--;
    return true;
}

void visualizar(Fila *f) {
    if (estaVazia(f)) {
        printf("Fila de espera vazia.\n");
        return;
    }
    printf("Fila de espera (ordem de atendimento):\n");
    int idx = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("  posicao %d | senha %d | %s\n",
               idx, f->dados[idx].senha, f->dados[idx].nome);
        idx = (idx + 1) % MAX;
    }
}

void lerLinha(char *destino, int tamanho) {
    int c;
    if (fgets(destino, tamanho, stdin) != NULL) {
        size_t n = strlen(destino);
        if (n > 0 && destino[n - 1] == '\n') {
            destino[n - 1] = '\0';
        } else {
            while ((c = getchar()) != '\n' && c != EOF);  /* descarta o excesso */
        }
    }
}

int main(void) {
    Fila fila;
    inicializar(&fila);

    int opcao = 0;
    int proximaSenha = 1;
    Pessoa p = {0, ""};
    char buffer[16];

    do {
        printf("\n===== FILA DE ATENDIMENTO =====\n");
        printf("1. Inserir pessoa\n");
        printf("2. Chamar proxima pessoa\n");
        printf("3. Visualizar fila de espera\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        lerLinha(buffer, sizeof(buffer));
        if (sscanf(buffer, "%d", &opcao) != 1) opcao = 0;

        switch (opcao) {
            case 1:
                if (estaCheia(&fila)) {
                    inserir(&fila, p);   /* apenas para exibir a mensagem de overflow */
                } else {
                    printf("Nome da pessoa: ");
                    lerLinha(p.nome, TAM_NOME);
                    p.senha = proximaSenha;
                    if (inserir(&fila, p)) proximaSenha++;
                }
                mostrarEstado(&fila);
                break;
            case 2:
                chamarProxima(&fila, &p);
                mostrarEstado(&fila);
                break;
            case 3:
                visualizar(&fila);
                mostrarEstado(&fila);
                break;
            case 4:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 4);

    return 0;
}
