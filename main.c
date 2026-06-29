#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grafo.h"

int main() {

    char nomeArquivo[100];
    printf("Digite o nome do arquivo do grafo: ");
    scanf("%s", nomeArquivo);

    int n = numero_vetores(nomeArquivo);
    int m = numero_arestas(nomeArquivo);

    if (n <= 0) {
        printf("Erro ao ler o grafo.\n");
        return 1;
    }
    int opcao;
    printf("\nEscolha a representacao do grafo:\n");
    printf("1 - Lista de adjacencia\n");
    printf("2 - Matriz de adjacencia\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    if (opcao != 1 && opcao != 2) {
        printf("Opcao invalida.\n");
        return 1;
    }

    int verticeInicial;
    printf("\nDigite o vertice inicial para a DFS (0 a %d): ", n - 1);
    scanf("%d", &verticeInicial);

    if (verticeInicial < 0 || verticeInicial >= n) {
        printf("Vertice invalido.\n");
        return 1;
    }

    Grafo_lista  *gLista  = NULL;
    Grafo_Matriz *gMatriz = NULL;

    if (opcao == 1) {
        gLista = iniciarGrafoLista(n, m);
        ler_inserir_lista(nomeArquivo, gLista);
        printf("\nGrafo carregado como lista de adjacencia.\n");
    } else {
        gMatriz = iniciarGrafoMatriz(n);
        ler_inserir_matriz(nomeArquivo, gMatriz);
        printf("\nGrafo carregado como matriz de adjacencia.\n");
    }

    Estatisticas stats;

    if (opcao == 1) {
        stats = estatisticas_lista(gLista);
    } else {
        stats = estatisticas_matriz(gMatriz);
    }

    int *pai   = malloc(n * sizeof(int));
    int *nivel = malloc(n * sizeof(int));

    clock_t inicioDFS, fimDFS;
    double tempoDFS;

    if (opcao == 1) {
        inicioDFS = clock();
        DFSLista(gLista, verticeInicial, pai, nivel);
        fimDFS = clock();
    } else {
        inicioDFS = clock();
        DFSMatriz(gMatriz, verticeInicial, pai, nivel);
        fimDFS = clock();
    }

    tempoDFS = (double)(fimDFS - inicioDFS) / CLOCKS_PER_SEC;

    int numComp;
    Componente *comp = NULL;

    clock_t inicioComp, fimComp;
    double tempoComp;

    if (opcao == 1) {
        inicioComp = clock();
        comp = componentesConexasLista(gLista, &numComp);
        fimComp = clock();
    } else {
        inicioComp = clock();
        comp = componentesConexasMatriz(gMatriz, &numComp);
        fimComp = clock();
    }

    tempoComp = (double)(fimComp - inicioComp) / CLOCKS_PER_SEC;

    int diametro;
    clock_t inicioDiam, fimDiam;
    double tempoDiam;

    if (opcao == 1) {
        inicioDiam = clock();
        diametro = diametroLista(gLista);
        fimDiam = clock();
    } else {
        inicioDiam = clock();
        diametro = diametroMatriz(gMatriz);
        fimDiam = clock();
    }

    tempoDiam = (double)(fimDiam - inicioDiam) / CLOCKS_PER_SEC;

    FILE *saida = fopen("saida_grafo.txt", "w");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saida.\n");
        return 1;
    }

    fprintf(saida, "================================================\n");
    fprintf(saida, "           RESULTADOS DO GRAFO\n");
    fprintf(saida, "================================================\n\n");
    fprintf(saida, "Arquivo de entrada : %s\n", nomeArquivo);
    fprintf(saida, "Representacao      : %s\n\n", opcao == 1 ? "Lista de adjacencia" : "Matriz de adjacencia");

    fprintf(saida, "================================================\n");
    fprintf(saida, "  ESTATISTICAS DO GRAFO\n");
    fprintf(saida, "================================================\n\n");
    fprintf(saida, "Numero de vertices : %d\n", stats.numVertices);
    fprintf(saida, "Numero de arestas  : %d\n", stats.numArestas);
    fprintf(saida, "Grau minimo        : %d\n", stats.grauMinimo);
    fprintf(saida, "Grau maximo        : %d\n", stats.grauMaximo);
    fprintf(saida, "Grau medio         : %.2f\n", stats.grauMedio);
    fprintf(saida, "Mediana de grau    : %.2f\n\n", stats.grauMediana);

    fprintf(saida, "================================================\n");
    fprintf(saida, "  DFS (ARVORE GERADORA)\n");
    fprintf(saida, "  Vertice inicial  : %d\n", verticeInicial);
    fprintf(saida, "  Tempo de execucao: %.6f segundos\n", tempoDFS);
    fprintf(saida, "================================================\n\n");
    fprintf(saida, "Vertice | Pai | Nivel\n");

    for (int i = 0; i < n; i++) {
        if (nivel[i] == -1) {
            // vertice nao alcancado pela DFS (esta em outra componente)
            fprintf(saida, "%7d | %3s | %5s\n", i, "-", "-");
        } else {
            fprintf(saida, "%7d | %3d | %5d\n", i, pai[i], nivel[i]);
        }
    }

    fprintf(saida, "\n================================================\n");
    fprintf(saida, "  COMPONENTES CONEXAS\n");
    fprintf(saida, "  Tempo de execucao: %.6f segundos\n", tempoComp);
    fprintf(saida, "================================================\n\n");
    fprintf(saida, "Numero de componentes: %d\n\n", numComp);

    for (int i = 0; i < numComp; i++) {
        fprintf(saida, "Componente %d (tamanho: %d)\n", i + 1, comp[i].tamanho);
        fprintf(saida, "Vertices: ");
        for (int j = 0; j < comp[i].tamanho; j++) {
            fprintf(saida, "%d ", comp[i].vertices[j]);
        }
        fprintf(saida, "\n\n");
    }

    fprintf(saida, "================================================\n");
    fprintf(saida, "  DIAMETRO DO GRAFO\n");
    fprintf(saida, "  Tempo de execucao: %.6f segundos\n", tempoDiam);
    fprintf(saida, "================================================\n\n");
    fprintf(saida, "Diametro: %d\n\n", diametro);

    fclose(saida);

    free(pai);
    free(nivel);

    for (int i = 0; i < numComp; i++) {
        free(comp[i].vertices);
    }
    free(comp);

    if (opcao == 1) {
        liberar_lista(gLista);
    } else {
        liberar_matriz(gMatriz, n);
    }

    printf("\nPronto! Resultados salvos em: saida_grafo.txt\n");

    return 0;
}