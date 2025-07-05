#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LINHAS 4
#define COLUNAS 2
#define TOTAL_CARTAS (LINHAS * COLUNAS)
#define MAX_TENTATIVAS 15

typedef struct {
    int valor;
    int visivel; // 0: escondido, 1: visível, 2: achado
} Carta;

// Função para embaralhar o vetor de valores
void embaralhar(int *valores, int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = valores[i];
        valores[i] = valores[j];
        valores[j] = temp;
    }
}

// Função para exibir o tabuleiro
void exibirTabuleiro(Carta *cartas) {
    printf("\nTabuleiro:\n");
    printf("   0 1\n");
    for (int i = 0; i < LINHAS; i++) {
        printf("%d  ", i);
        for (int j = 0; j < COLUNAS; j++) {
            Carta *c = cartas + (i * COLUNAS + j);
            if (c->visivel == 1 || c->visivel == 2)
                printf("%d ", c->valor);
            else
                printf("* ");
        }
        printf("\n");
    }
    printf("\n");
}

// Função para verificar se todos os pares foram encontrados
int todosParesAchados(Carta *cartas) {
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        if ((cartas + i)->visivel != 2)
            return 0;
    }
    return 1;
}

// Função para salvar resultado em arquivo
void salvarResultado(int tentativas, int tempo) {
    FILE *fp = fopen("resultado_memoria.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "Tentativas: %d | Tempo: %d segundos\n", tentativas, tempo);
        fclose(fp);
    }
}

int main() {
    Carta cartas[LINHAS][COLUNAS];
    int valores[TOTAL_CARTAS] = {1,1,2,2,3,3,4,4};
    int tentativas = 0;
    int linha1, col1, linha2, col2;
    time_t inicio, fim;

    srand((unsigned)time(NULL));
    embaralhar(valores, TOTAL_CARTAS);

    // Preenche o tabuleiro
    for (int i = 0, k = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++, k++) {
            cartas[i][j].valor = valores[k];
            cartas[i][j].visivel = 0;
        }
    }

    printf("Bem-vindo ao Jogo da Memória (Pares)!\n");
    printf("Encontre todos os pares em no máximo %d tentativas.\n", MAX_TENTATIVAS);

    inicio = time(NULL);

    while (tentativas < MAX_TENTATIVAS && !todosParesAchados(&cartas[0][0])) {
        exibirTabuleiro(&cartas[0][0]);

        printf("Tentativa %d\n", tentativas + 1);

        // Primeira carta
        do {
            printf("Escolha a primeira carta (linha coluna): ");
            scanf("%d %d", &linha1, &col1);
        } while (linha1 < 0 || linha1 >= LINHAS || col1 < 0 || col1 >= COLUNAS || cartas[linha1][col1].visivel == 2 || cartas[linha1][col1].visivel == 1);

        cartas[linha1][col1].visivel = 1;
        exibirTabuleiro(&cartas[0][0]);

        // Segunda carta
        do {
            printf("Escolha a segunda carta (linha coluna): ");
            scanf("%d %d", &linha2, &col2);
        } while (linha2 < 0 || linha2 >= LINHAS || col2 < 0 || col2 >= COLUNAS || (linha2 == linha1 && col2 == col1) || cartas[linha2][col2].visivel == 2 || cartas[linha2][col2].visivel == 1);

        cartas[linha2][col2].visivel = 1;
        exibirTabuleiro(&cartas[0][0]);

        if (cartas[linha1][col1].valor == cartas[linha2][col2].valor) {
            printf("Par encontrado!\n");
            cartas[linha1][col1].visivel = 2;
            cartas[linha2][col2].visivel = 2;
        } else {
            printf("Não é um par.\n");
            cartas[linha1][col1].visivel = 0;
            cartas[linha2][col2].visivel = 0;
        }

        tentativas++;
    }

    fim = time(NULL);

    if (todosParesAchados(&cartas[0][0])) {
        printf("Parabéns! Você encontrou todos os pares em %d tentativas!\n", tentativas);
    } else {
        printf("Fim de jogo! Você não encontrou todos os pares.\n");
    }
    printf("Tempo decorrido: %ld segundos\n", fim - inicio);

    salvarResultado(tentativas, (int)(fim - inicio));
    printf("Resultado salvo em resultado_memoria.txt\n");

    return 0;
}