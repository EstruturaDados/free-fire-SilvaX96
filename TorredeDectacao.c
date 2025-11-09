/* detector_tower_sort.c
 * Programa educativo para gerenciar componentes da torre de resgate.
 * Implementa: bubble sort por nome, insertion sort por tipo, selection sort por prioridade,
 * busca binária por nome (após ordenar por nome), medição de comparações e tempo com clock().
 * Limite: até 20 componentes.
 * Compilar: gcc -std=c99 -O2 -o tower detector_tower_sort.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define NOME_TAM 30
#define TIPO_TAM 20

typedef struct {
    char nome[NOME_TAM];
    char tipo[TIPO_TAM];
    int prioridade;
} Componente;

/* --- Assinaturas --- */
void bubbleSortNome(Componente arr[], int n, long *comparisons);
void insertionSortTipo(Componente arr[], int n, long *comparisons);
void selectionSortPrioridade(Componente arr[], int n, long *comparisons);
int buscaBinariaPorNome(Componente arr[], int n, const char key[], long *comparisons);
void mostrarComponentes(Componente arr[], int n);
double medirTempo(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparisons);

/* --- Implementações --- */

/* Remove newline de string trazida por fgets */
void strip_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

/* Troca dois componentes */
void swapComponente(Componente *a, Componente *b) {
    Componente tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Bubble sort por nome (lexicográfico). Conta comparações de nome (strcmp). */
void bubbleSortNome(Componente arr[], int n, long *comparisons) {
    *comparisons = 0;
    if (n <= 1) return;
    int i, j;
    for (i = 0; i < n-1; ++i) {
        int trocou = 0;
        for (j = 0; j < n-1-i; ++j) {
            (*comparisons)++; // comparando arr[j].nome com arr[j+1].nome
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                swapComponente(&arr[j], &arr[j+1]);
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

/* Insertion sort por tipo (string). Conta comparações de tipo. */
void insertionSortTipo(Componente arr[], int n, long *comparisons) {
    *comparisons = 0;
    for (int i = 1; i < n; ++i) {
        Componente key = arr[i];
        int j = i - 1;
        // enquanto j >= 0 e arr[j].tipo > key.tipo
        while (j >= 0) {
            (*comparisons)++;
            if (strcmp(arr[j].tipo, key.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = key;
    }
}

/* Selection sort por prioridade (int). Conta comparações inteiras. */
void selectionSortPrioridade(Componente arr[], int n, long *comparisons) {
    *comparisons = 0;
    for (int i = 0; i < n-1; ++i) {
        int min_idx = i;
        for (int j = i+1; j < n; ++j) {
            (*comparisons)++;
            if (arr[j].prioridade < arr[min_idx].prioridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) swapComponente(&arr[i], &arr[min_idx]);
    }
}

/* Busca binária por nome. Retorna índice ou -1. Conta comparações (cada strcmp chama incrementa). */
int buscaBinariaPorNome(Componente arr[], int n, const char key[], long *comparisons) {
    int left = 0, right = n - 1;
    *comparisons = 0;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        (*comparisons)++;
        int cmp = strcmp(arr[mid].nome, key);
        if (cmp == 0) return mid;
        else if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

/* Mostra tabela de componentes */
void mostrarComponentes(Componente arr[], int n) {
    if (n == 0) {
        printf("(Nenhum componente cadastrado)\n");
        return;
    }
    printf("\nLista de Componentes (%d):\n", n);
    printf("%-3s | %-28s | %-18s | %-9s\n", "#", "Nome", "Tipo", "Prioridade");
    printf("----+------------------------------+--------------------+----------\n");
    for (int i = 0; i < n; ++i) {
        printf("%3d | %-28s | %-18s | %9d\n", i, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("\n");
}

/* medirTempo: recebe uma função de ordenação (arr, n, &comparisons) e devolve tempo em segundos
 * comparisons é preenchido pela função de ordenação.
 */
double medirTempo(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparisons) {
    clock_t start = clock();
    algoritmo(arr, n, comparisons);
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

/* Menu e utilitários de entrada */
int lerInteiro(const char *prompt, int min, int max) {
    char buf[64];
    int val;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) return min;
        if (sscanf(buf, "%d", &val) == 1) {
            if (val >= min && val <= max) return val;
            printf("Valor deve estar entre %d e %d. Tente novamente.\n", min, max);
        } else {
            printf("Entrada inválida. Digite um número.\n");
        }
    }
}

void lerString(const char *prompt, char *dest, int size) {
    printf("%s", prompt);
    if (fgets(dest, size, stdin) == NULL) dest[0] = '\0';
    strip_newline(dest);
}

/* Programa principal */
int main(void) {
    Componente componentes[MAX_COMPONENTES];
    int n = 0;
    int ordenado_por_nome = 0;

    printf("=== Sistema de Priorizacao e Montagem da Torre de Resgate ===\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1) Cadastrar componente (max %d)\n", MAX_COMPONENTES);
        printf("2) Mostrar componentes\n");
        printf("3) Ordenar por NOME (Bubble Sort)\n");
        printf("4) Ordenar por TIPO (Insertion Sort)\n");
        printf("5) Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("6) Busca binaria por NOME (apenas apos ordenar por nome)\n");
        printf("7) Montagem final (exibir e checar componente-chave)\n");
        printf("0) Sair\n");

        int opc = lerInteiro("Escolha uma opcao: ", 0, 7);

        if (opc == 0) {
            printf("Saindo... boa sorte na fuga!\n");
            break;
        }

        if (opc == 1) {
            if (n >= MAX_COMPONENTES) {
                printf("Limite de componentes atingido (%d).\n", MAX_COMPONENTES);
                continue;
            }
            Componente c;
            lerString("Nome: ", c.nome, NOME_TAM);
            lerString("Tipo: ", c.tipo, TIPO_TAM);
            c.prioridade = lerInteiro("Prioridade (1-10): ", 1, 10);
            componentes[n++] = c;
            ordenado_por_nome = 0; // qualquer insercao invalida ordenacao por nome
            printf("Componente cadastrado com sucesso.\n");
        }
        else if (opc == 2) {
            mostrarComponentes(componentes, n);
        }
        else if (opc == 3) {
            if (n == 0) { printf("Nenhum componente para ordenar.\n"); continue; }
            Componente copia[MAX_COMPONENTES];
            memcpy(copia, componentes, sizeof(Componente)*n);
            long comps = 0;
            double t = medirTempo(bubbleSortNome, copia, n, &comps);
            printf("\nResultado (apenas visual, vetor original NAO alterado):\n");
            mostrarComponentes(copia, n);
            printf("Bubble Sort por NOME: comparacoes=%ld, tempo=%.6f s\n", comps, t);
            // aplicar ao vetor original para que futuras operacoes usem a ordenacao
            bubbleSortNome(componentes, n, &comps);
            ordenado_por_nome = 1;
            printf("Ordenacao aplicada ao vetor principal.\n");
        }
        else if (opc == 4) {
            if (n == 0) { printf("Nenhum componente para ordenar.\n"); continue; }
            Componente copia[MAX_COMPONENTES];
            memcpy(copia, componentes, sizeof(Componente)*n);
            long comps = 0;
            double t = medirTempo(insertionSortTipo, copia, n, &comps);
            printf("\nResultado (apenas visual):\n");
            mostrarComponentes(copia, n);
            printf("Insertion Sort por TIPO: comparacoes=%ld, tempo=%.6f s\n", comps, t);
            // aplicar ao vetor original
            insertionSortTipo(componentes, n, &comps);
            ordenado_por_nome = 0;
            printf("Ordenacao por tipo aplicada ao vetor principal.\n");
        }
        else if (opc == 5) {
            if (n == 0) { printf("Nenhum componente para ordenar.\n"); continue; }
            Componente copia[MAX_COMPONENTES];
            memcpy(copia, componentes, sizeof(Componente)*n);
            long comps = 0;
            double t = medirTempo(selectionSortPrioridade, copia, n, &comps);
            printf("\nResultado (apenas visual):\n");
            mostrarComponentes(copia, n);
            printf("Selection Sort por PRIORIDADE: comparacoes=%ld, tempo=%.6f s\n", comps, t);
            // aplicar ao vetor original
            selectionSortPrioridade(componentes, n, &comps);
            ordenado_por_nome = 0;
            printf("Ordenacao por prioridade aplicada ao vetor principal.\n");
        }
        else if (opc == 6) {
            if (!ordenado_por_nome) {
                printf("Busca binaria exige que o vetor esteja ordenado por NOME. Use a opcao 3 primeiro.\n");
                continue;
            }
            char chave[NOME_TAM];
            lerString("Digite o nome do componente a buscar: ", chave, NOME_TAM);
            long comps = 0;
            clock_t s = clock();
            int idx = buscaBinariaPorNome(componentes, n, chave, &comps);
            clock_t e = clock();
            double tempo = ((double)(e - s)) / CLOCKS_PER_SEC;
            if (idx >= 0) {
                printf("Componente encontrado no indice %d:\n", idx);
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n", componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
            } else printf("Componente nao encontrado.\n");
            printf("Comparacoes na busca binaria: %ld, tempo=%.6f s\n", comps, tempo);
        }
        else if (opc == 7) {
            if (n == 0) { printf("Nenhum componente cadastrado para montagem.\n"); continue; }
            printf("\nEscolha a estrategia que voce quer aplicar para a montagem final:\n");
            printf("1) Por NOME (Bubble Sort)\n");
            printf("2) Por TIPO (Insertion Sort)\n");
            printf("3) Por PRIORIDADE (Selection Sort)\n");
            int e = lerInteiro("Opcao: ", 1, 3);
            long comps = 0;
            double tempo = 0.0;
            if (e == 1) tempo = medirTempo(bubbleSortNome, componentes, n, &comps), ordenado_por_nome = 1;
            else if (e == 2) tempo = medirTempo(insertionSortTipo, componentes, n, &comps), ordenado_por_nome = 0;
            else if (e == 3) tempo = medirTempo(selectionSortPrioridade, componentes, n, &comps), ordenado_por_nome = 0;

            printf("\nComponentes ordenados segundo a estrategia escolhida:\n");
            mostrarComponentes(componentes, n);
            printf("Comparacoes=%ld, tempo=%.6f s\n", comps, tempo);

            // Confirma presença do componente-chave
            char chave[NOME_TAM];
            lerString("Digite o NOME do componente-chave que destrava a torre: ", chave, NOME_TAM);
            long compsBusca = 0;
            int idx = -1;
            if (ordenado_por_nome) {
                clock_t s = clock();
                idx = buscaBinariaPorNome(componentes, n, chave, &compsBusca);
                clock_t e2 = clock();
                double t2 = ((double)(e2 - s)) / CLOCKS_PER_SEC;
                printf("Busca binaria (vetor ordenado por nome) -> comparacoes=%ld, tempo=%.6f s\n", compsBusca, t2);
            } else {
                // busca linear se nao estiver ordenado por nome
                compsBusca = 0;
                clock_t s = clock();
                for (int i = 0; i < n; ++i) {
                    compsBusca++;
                    if (strcmp(componentes[i].nome, chave) == 0) { idx = i; break; }
                }
                clock_t e2 = clock();
                double t2 = ((double)(e2 - s)) / CLOCKS_PER_SEC;
                printf("Busca linear (vetor NAO ordenado por nome) -> comparacoes=%ld, tempo=%.6f s\n", compsBusca, t2);
            }

            if (idx >= 0) {
                printf("\nComponente-chave PRESENTE! Indice=%d. Torre pode ser ativada.\n", idx);
            } else printf("\nComponente-chave AUSENTE. Montagem falha.\n");
        }
        else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}
