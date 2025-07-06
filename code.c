#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definindo o tamanho do tabuleiro e limites do jogo
#define LINHAS 4
#define COLUNAS 2
#define TOTAL_CARTAS (LINHAS * COLUNAS)
#define MAX_TENTATIVAS 15

// Estrutura representando cada carta do tabuleiro
typedef struct {
    int valor;    // Valor numérico do par (ex: 1, 2, 3, 4)
    int visivel;  // 0: escondido, 1: temporariamente visível, 2: par encontrado
} Carta;

// Função que embaralha um vetor de inteiros usando o algoritmo Fisher-Yates
void embaralhar(int *valores, int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Escolhe índice aleatório entre 0 e i
        int temp = valores[i];
        valores[i] = valores[j];
        valores[j] = temp;
    }
}

// Exibe o tabuleiro atual para o usuário
// Mostra "*" para cartas escondidas, mostra números para cartas visíveis/encontradas
void exibirTabuleiro(Carta *cartas) {
    printf("\nTabuleiro:\n");
    printf("   0 1\n"); // Cabeçalho das colunas
    for (int i = 0; i < LINHAS; i++) {
        printf("%d  ", i); // Número da linha
        for (int j = 0; j < COLUNAS; j++) {
            Carta *c = cartas + (i * COLUNAS + j);
            if (c->visivel == 1 || c->visivel == 2)
                printf("%d ", c->valor); // Mostra valor se visível ou já encontrado
            else
                printf("* "); // Esconde valor
        }
        printf("\n");
    }
    printf("\n");
}

// Verifica se todos os pares já foram encontrados
int todosParesAchados(Carta *cartas) {
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        if ((cartas + i)->visivel != 2)
            return 0; // Ainda há pelo menos uma carta não encontrada
    }
    return 1; // Todos os pares foram encontrados
}

// Salva o resultado da partida (tentativas e tempo) em um arquivo de texto
void salvarResultado(int tentativas, int tempo) {
    FILE *fp = fopen("resultado_memoria.txt", "a"); // Abre arquivo para acrescentar
    if (fp != NULL) {
        fprintf(fp, "Tentativas: %d | Tempo: %d segundos\n", tentativas, tempo);
        fclose(fp);
    } else {
        // Tratamento de erro caso o arquivo não possa ser aberto
        printf("Erro ao salvar resultado no arquivo!\n");
    }
}

int main() {
    Carta cartas[LINHAS][COLUNAS];      // Matriz representando o tabuleiro
    int valores[TOTAL_CARTAS] = {1,1,2,2,3,3,4,4}; // Pares de valores
    int tentativas = 0;                 // Contador de tentativas do jogador
    int linha1, col1, linha2, col2;     // Coordenadas escolhidas pelo jogador
    time_t inicio, fim;                 // Para calcular tempo da partida

    srand((unsigned)time(NULL));        // Inicializa gerador de números aleatórios

    embaralhar(valores, TOTAL_CARTAS);  // Embaralha os pares

    // Preenche a matriz de cartas, atribuindo valores embaralhados e marcando como escondidas
    for (int i = 0, k = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++, k++) {
            cartas[i][j].valor = valores[k];
            cartas[i][j].visivel = 0;
        }
    }

    // Mensagem inicial ao jogador
    printf("Bem-vindo ao Jogo da Memória (Pares)!\n");
    printf("Encontre todos os pares em no máximo %d tentativas.\n", MAX_TENTATIVAS);

    inicio = time(NULL); // Marca o início da partida

    // Loop principal: continua até encontrar todos os pares ou atingir o máximo de tentativas
    while (tentativas < MAX_TENTATIVAS && !todosParesAchados(&cartas[0][0])) {
        exibirTabuleiro(&cartas[0][0]); // Mostra o tabuleiro atual

        printf("Tentativa %d\n", tentativas + 1);

        // Entrada da primeira carta
        do {
            printf("Escolha a primeira carta (linha coluna): ");
            scanf("%d %d", &linha1, &col1);
            // Valida se a escolha está dentro do tabuleiro e se a carta não está visível/finalizada
            if (linha1 < 0 || linha1 >= LINHAS || col1 < 0 || col1 >= COLUNAS ||
                cartas[linha1][col1].visivel == 2 || cartas[linha1][col1].visivel == 1) {
                printf("Posição inválida. Tente novamente.\n");
            }
        } while (linha1 < 0 || linha1 >= LINHAS || col1 < 0 || col1 >= COLUNAS ||
                 cartas[linha1][col1].visivel == 2 || cartas[linha1][col1].visivel == 1);

        cartas[linha1][col1].visivel = 1; // Revela temporariamente a carta escolhida
        exibirTabuleiro(&cartas[0][0]);   // Mostra o tabuleiro após escolha

        // Entrada da segunda carta
        do {
            printf("Escolha a segunda carta (linha coluna): ");
            scanf("%d %d", &linha2, &col2);
            // Valida se é diferente da primeira, está no tabuleiro e não está visível/finalizada
            if (linha2 < 0 || linha2 >= LINHAS || col2 < 0 || col2 >= COLUNAS ||
                (linha2 == linha1 && col2 == col1) ||
                cartas[linha2][col2].visivel == 2 || cartas[linha2][col2].visivel == 1) {
                printf("Posição inválida. Tente novamente.\n");
            }
        } while (linha2 < 0 || linha2 >= LINHAS || col2 < 0 || col2 >= COLUNAS ||
                 (linha2 == linha1 && col2 == col1) ||
                 cartas[linha2][col2].visivel == 2 || cartas[linha2][col2].visivel == 1);

        cartas[linha2][col2].visivel = 1; // Revela a segunda carta
        exibirTabuleiro(&cartas[0][0]);   // Mostra o tabuleiro após escolha

        // Checa se as duas cartas formam um par
        if (cartas[linha1][col1].valor == cartas[linha2][col2].valor) {
            printf("Par encontrado!\n");
            cartas[linha1][col1].visivel = 2; // Marca como par achado
            cartas[linha2][col2].visivel = 2;
        } else {
            printf("Não é um par.\n");
            cartas[linha1][col1].visivel = 0; // Esconde novamente
            cartas[linha2][col2].visivel = 0;
        }

        tentativas++; // Incrementa contador de tentativas
    }

    fim = time(NULL); // Marca o final da partida

    // Mensagem final: vitória ou derrota
    if (todosParesAchados(&cartas[0][0])) {
        printf("Parabéns! Você encontrou todos os pares em %d tentativas!\n", tentativas);
    } else {
        printf("Fim de jogo! Você não encontrou todos os pares.\n");
    }
    printf("Tempo decorrido: %ld segundos\n", fim - inicio);

    // Salva o resultado da partida no arquivo
    salvarResultado(tentativas, (int)(fim - inicio));
    printf("Resultado salvo em resultado_memoria.txt\n");

    return 0;
}