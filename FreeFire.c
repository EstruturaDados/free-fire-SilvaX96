#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

// Struct que representa um item do inventário
typedef struct {
    char nome[MAX_NOME];     // Nome do item
    char tipo[MAX_TIPO];     // Tipo do item (ex: arma, munição, cura)
    int quantidade;          // Quantidade do item
} Item;

// Protótipos das funções
void inserirItem(Item mochila[], int *numItens);
void removerItem(Item mochila[], int *numItens);
void listarItens(Item mochila[], int numItens);
void buscarItem(Item mochila[], int numItens);

// Função para remover o \n do fgets
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int main() {
    Item mochila[MAX_ITENS];
    int numItens = 0;
    int opcao;

    printf("=== Sistema de Inventário - Mochila de Loot Inicial ===\n\n");

    do {
        printf("\nMenu:\n");
        printf("1) Inserir item\n");
        printf("2) Remover item\n");
        printf("3) Listar itens\n");
        printf("4) Buscar item\n");
        printf("0) Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer do enter

        switch(opcao) {
            case 1:
                inserirItem(mochila, &numItens);
                break;
            case 2:
                removerItem(mochila, &numItens);
                break;
            case 3:
                listarItens(mochila, numItens);
                break;
            case 4:
                buscarItem(mochila, numItens);
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while(opcao != 0);

    return 0;
}

// Função para inserir um novo item na mochila
void inserirItem(Item mochila[], int *numItens) {
    if (*numItens >= MAX_ITENS) {
        printf("Mochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novoItem;

    printf("\nNome do item: ");
    fgets(novoItem.nome, MAX_NOME, stdin);
    trim_newline(novoItem.nome);

    printf("Tipo do item (arma, munição, cura, etc): ");
    fgets(novoItem.tipo, MAX_TIPO, stdin);
    trim_newline(novoItem.tipo);

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    getchar();

    mochila[*numItens] = novoItem;
    (*numItens)++;

    printf("Item adicionado com sucesso!\n");
    listarItens(mochila, *numItens);
}

// Função para remover um item pelo nome
void removerItem(Item mochila[], int *numItens) {
    if (*numItens == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    char nome[MAX_NOME];
    printf("\nDigite o nome do item a ser removido: ");
    fgets(nome, MAX_NOME, stdin);
    trim_newline(nome);

    for (int i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            for (int j = i; j < *numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*numItens)--;
            printf("Item '%s' removido com sucesso!\n", nome);
            listarItens(mochila, *numItens);
            return;
        }
    }

    printf("Item não encontrado!\n");
}

// Função para listar todos os itens da mochila
void listarItens(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("\nMochila está vazia!\n");
        return;
    }

    printf("\nItens na mochila:\n");
    printf("+----+------------------------------+----------------------+------------+\n");
    printf("| #  | Nome                         | Tipo                 | Quantidade |\n");
    printf("+----+------------------------------+----------------------+------------+\n");

    for (int i = 0; i < numItens; i++) {
        printf("| %2d | %-28s | %-20s | %10d |\n", i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }

    printf("+----+------------------------------+----------------------+------------+\n");
}

// Função de busca sequencial por nome
void buscarItem(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("\nMochila está vazia!\n");
        return;
    }

    char nome[MAX_NOME];
    printf("\nDigite o nome do item a buscar: ");
    fgets(nome, MAX_NOME, stdin);
    trim_newline(nome);

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("Item '%s' não encontrado na mochila.\n", nome);
}