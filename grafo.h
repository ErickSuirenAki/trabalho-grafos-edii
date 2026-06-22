#ifndef GRAFO_H
#define GRAFO_H
#include <stdbool.h>

typedef struct Vertice {
    int id;
    struct Vertice *prox;
} Vertice;
typedef struct{
    int *dados;
    int inicio;
    int fim;
    int tamanho;
}Fila;
typedef struct {
    int numVertices;
    int numArestas;
    Vertice **listaAdj;
}Grafo_lista;
typedef struct{
    int numVertices;
    int numArestas;
    int **matriz;
}Grafo_Matriz;
typedef struct{
    int numVertices;
    int numArestas;
    int grauMinimo;
    int grauMaximo;
    double grauMedio;
    double grauMediana;
}Estatisticas;
typedef struct{
    int *idx;
    int tamanho;
}ListaVertice;


//implementacao da fila
Fila *criarFila(int tamanho);
void enfilerar(Fila *f,int vertice);
int desenfilerar(Fila *f);
bool filaVazia(Fila *f);
ListaVertice BFS(Grafo_lista *grafo,int inicio);


// funções

//lista adjascente
Grafo_lista *iniciarGrafoLista(int numVertices);
Vertice* criarVertice_lista(int idt);
void inserirInicio(Grafo_lista *grafo, Vertice *vertice,int index);
void insercao_aresta_lista(Grafo_lista *grafo, int origem, int destino);
void liberar_lista(Grafo_lista *grafo);
void imprimir_grafo_lista(Grafo_lista *grafo);

//matriz adjascente
Grafo_Matriz *iniciarGrafoMatriz(int numVertices);
void liberar_matriz(Grafo_Matriz *grafo, int tam);
void insercao_aresta_matriz(Grafo_Matriz *grafo, int origem, int destino);

//ler arquivo
int numero_vetores(char nome_arq[]);
void ler_inserir(char nome_arq[], Grafo_lista *grafoAdj, Grafo_Matriz *grafoMat);

//ordencao e mediana
int comp(const void * a, const void *b);
double mediana(int vetor[],int tam);

//estastisticas grafo lista
int grau_vertice_lista(Grafo_lista *grafo, int n);
Estatisticas estatisticas_lista(Grafo_lista *grafo);
//estastisticas grafo matriz
int grau_vertice_matriz(Grafo_Matriz *grafo, int vertice);
Estatisticas estatisticas_matriz(Grafo_Matriz *grafo);


#endif