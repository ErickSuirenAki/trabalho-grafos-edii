#ifndef GRAFO_H
#define GRAFO_H

typedef struct Vertice {
    int id;
    struct Vertice *prox;
} Vertice;

typedef struct Grafo_lista {
    int numVertices;
    int numArestas;
    Vertice **listaAdj;
} Grafo_lista;

// funções
Grafo_lista *inicializarGrafoLista(int numVertices);
Vertice* criarVertice_lista(int idt);
void inserirInicio(Vertice **listaVertice, Vertice *vertice, int index);
void insere_lista(Grafo_lista *grafo, int origem, int destino);

int **criarMatriz(int tamanho);
void inserir_matriz(int **matriz, int origem, int destino);

int numero_vetores(char nome_arq[]);
void ler_inserir(char nome_arq[], Grafo_lista *grafo, int **matriz);

#endif