#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>


void teste(Estatisticas est){
    printf("Grau Minimo: %d\n",est.grauMinimo);
    printf("Grau Medio: %f\n",est.grauMedio);
    printf("Grau Mediana: %f\n",est.grauMediana);
    printf("Grau Maximo: %d\n",est.grauMaximo);
    printf("Numero de vertices: %d\n",est.numVertices);
    printf("Numero de arestas: %d\n",est.numArestas);
}


int main(int argc, char const *argv[])
{

    char nome_arquivo[] = "grafo_1.txt";

    int TAM = numero_vetores(nome_arquivo);
    if (TAM <= 0) {
        printf("Erro ao ler tamanho do grafo.\n");
        return 1;
    }

    Grafo_lista *grafoAdj = iniciarGrafoLista(TAM);
    if (grafoAdj == NULL) {
        printf("Erro ao inicializar grafo.\n");
        return 1;
    }

   Grafo_Matriz *grafoMat = iniciarGrafoMatriz(TAM);
    if (grafoMat == NULL) {
        printf("Erro ao criar matriz.\n");
        return 1;
    }

    ler_inserir(nome_arquivo, grafoAdj, grafoMat);  
    printf("Grafo carregado com sucesso!\n");

    Estatisticas estLista = estatisticas_lista(grafoAdj);
    printf("Estastiticas lista carregado com sucesso!\n");

    Estatisticas estMatriz = estatisticas_matriz(grafoMat);
    printf("Estastiticas matriz carregado com sucesso!\n");

    puts("\nImprimindo estastiticas Listas:");
    teste(estLista);
    puts("\nImprimindo estastiticas Matrizes:");
    teste(estMatriz);

    liberar_lista(grafoAdj);
    puts("\nGrafo lista liberado com sucesso!");
    liberar_matriz(grafoMat,TAM);
    puts("\nGrafo matriz liberado com sucesso!");

    return 0;
}