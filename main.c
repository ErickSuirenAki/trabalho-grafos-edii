#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

int main() {

    char nomeArquivo[] = "grafo.txt";
    int n = numero_vetores(nomeArquivo);

    if (n <= 0) {
        printf("Erro ao ler o grafo.\n");
        return 1;
    }
    int opcao;
    printf("Escolha a representacao do grafo:\n");
    printf("1 - Lista de adjacencia\n");
    printf("2 - Matriz de adjacencia\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    Grafo_lista *gLista = NULL;
    Grafo_Matriz *gMatriz = NULL;

    if (opcao == 1) {
        gLista = iniciarGrafoLista(n);
        ler_inserir(nomeArquivo, gLista, NULL);
    }
    else if (opcao == 2) {
        gMatriz = iniciarGrafoMatriz(n);
        ler_inserir(nomeArquivo, NULL, gMatriz);
    }
    else {
        printf("Opcao invalida.\n");
        return 1;
    }

    FILE *saida = fopen("saida_grafo.txt", "w");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saida.\n");
        return 1;
    }

    fprintf(saida, "SAIDA DO GRAFO\n\n");

    int *pai = malloc(n * sizeof(int));
    int *nivel = malloc(n * sizeof(int));

    int numComp;
    Componente *comp = NULL;

    if (opcao == 1) {

        DFSLista(gLista, 0, pai, nivel);

        comp = componentesConexasLista(gLista, &numComp);

    }

    else {

        DFSMatriz(gMatriz, 0, pai, nivel);

        comp = componentesConexasMatriz(gMatriz, &numComp);
    }

    fprintf(saida, "DFS (ARVORE GERADORA)\n");
    fprintf(saida, "Vertice | Pai | Nivel\n");

    for (int i = 0; i < n; i++) {
        fprintf(saida, "%d %d %d\n", i, pai[i], nivel[i]);
    }

    fprintf(saida, "\n");

    fprintf(saida, "COMPONENTES CONEXAS\n");
    fprintf(saida, "Numero de componentes: %d\n\n", numComp);

    for (int i = 0; i < numComp; i++) {
        fprintf(saida, "Componente %d (tamanho %d): ",
                i + 1,
                comp[i].tamanho);

        for (int j = 0; j < comp[i].tamanho; j++) {
            fprintf(saida, "%d ", comp[i].vertices[j]);
        }

        fprintf(saida, "\n");
    }

    fclose(saida);

    free(pai);
    free(nivel);

    if (opcao == 1) {
        liberar_lista(gLista);
    } else {
        liberar_matriz(gMatriz, n);
    }

    printf("Arquivo gerado: saida_grafo.txt\n");

    return 0;
}