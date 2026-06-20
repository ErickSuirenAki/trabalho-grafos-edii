#include <stdio.h>
#include "grafo.h"
#include <stdlib.h>

Grafo_lista *inicializarGrafoLista(int numVertices){
    Grafo_lista *grafo = malloc(sizeof(Grafo_lista));
    
    if(grafo == NULL) {
        free(grafo);
        return NULL;
    }
    
    grafo->listaAdj = calloc(numVertices, sizeof(Vertice*));
    
      if(grafo->listaAdj == NULL) {
        free(grafo->listaAdj);
        free(grafo);
        return NULL;
    }

    grafo->numVertices = numVertices;
    grafo->numArestas = 0;
    
    return grafo;
}

Vertice* criarVertice_lista(int idt){
	Vertice *novoVertice = malloc(sizeof(Vertice));
	novoVertice->id = idt;
	novoVertice->prox = NULL;
	return novoVertice;
}
void inserirInicio(Vertice **listaVertice, Vertice *vertice,int index){
    Vertice *aux = listaVertice[index];
    listaVertice[index] = vertice;
    vertice->prox = aux; 
}
void insere_lista(Grafo_lista *grafo, int origem, int destino){
	Vertice *novoVertice = criarVertice_lista(destino);
    Vertice *simetricoVertice = criarVertice_lista(origem);
    
	if (novoVertice == NULL || simetricoVertice == NULL) {
        printf("Erro: sem memória para inserir aresta (%d, %d)\n", origem, destino);
        free(novoVertice);       
        free(simetricoVertice);
        return;
    }
    inserirInicio(grafo->listaAdj, novoVertice, origem);
    inserirInicio(grafo->listaAdj, simetricoVertice, destino);
    grafo->numArestas++;
}


int **criarMatriz(int tamanho) {
    int **matriz;
    int i;

    // Aloca o vetor de ponteiros
    matriz = (int **)calloc(tamanho, sizeof(int *));
    if (matriz == NULL)
        return NULL;

    // Aloca cada linha já preenchida com 0
    for (i = 0; i < tamanho; i++) {
        matriz[i] = (int *)calloc(tamanho, sizeof(int));

        if (matriz[i] == NULL) {
            while (--i >= 0)
                free(matriz[i]);

            free(matriz);
            return NULL;
        }
    }

    return matriz;
}
void inserir_matriz(int **matriz, int origem, int destino){
    matriz[origem][destino] = 1;
    matriz[destino][origem] = 1;
}


int numero_vetores(char nome_arq[]){
    FILE *file = fopen(nome_arq,"r");
    if(file == NULL){
        printf("ERRO, não foi possivel abrir o arquivo: %s",nome_arq);
        return -1;
    }else{
        int tam;
        fscanf(file,"%d",&tam);
        return tam;
    }
}
void ler_inserir(char nome_arq[], Grafo_lista *grafo, int **matriz){
    FILE *file = fopen(nome_arq,"r");
    if(file == NULL){
        printf("ERRO, não foi possivel abrir o arquivo: %s",nome_arq);
        return;
    }else{
        int origem, destino,lixo;
        fscanf(file,"%d",&lixo);
        while(fscanf(file,"%d %d",&origem,&destino) == 2){
            inserir_matriz(matriz,origem-1,destino-1);
            insere_lista(grafo, origem-1, destino-1);
        }
        fclose(file);
    }
}

