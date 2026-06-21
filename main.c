#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    printf("Executando...");
    char nome_arquivo[] = "grafo_1.txt";
    printf("1...");
    int TAM = numero_vetores(nome_arquivo);
    if (TAM <= 0) {
        printf("Erro ao ler tamanho do grafo.\n");
        return 1;
    }
    printf("2...");
    Grafo_lista *grafoL = inicializarGrafoLista(TAM);
    if (grafoL == NULL) {
        printf("Erro ao inicializar grafo.\n");
        return 1;
    }
    printf("3...");
    int **matriz = criarMatriz(TAM);
    if (matriz == NULL) {
        printf("Erro ao criar matriz.\n");
        return 1;
    }
    printf("4...");
    ler_inserir(nome_arquivo, grafoL, matriz);
    printf("5...");
    printf("numero de arestas: %d",grafoL->numArestas);
    printf("Grafo carregado com sucesso!\n");

    return 0;
}