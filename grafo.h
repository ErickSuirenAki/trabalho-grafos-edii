#ifndef GRAFO_H
#define GRAFO_H
#include <stdbool.h>
#include<time.h>

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
    int tam;
    int numArestas;
    Vertice **listaAdj;
    Vertice *pool;
    int *graus;
    int index_pool;
}Grafo_lista;

typedef struct{
    int tam;
    int numArestas;
    bool **matriz;
    int *graus;
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
}ResultadoBFS;


//implementacao da fila
Fila *criarFila(int tamanho);
void enfilerar(Fila *f,int vertice);
int desenfilerar(Fila *f);
bool filaVazia(Fila *f);

ResultadoBFS BFS_lista(Grafo_lista *grafo,int inicio);
ResultadoBFS BFS_matriz(Grafo_Matriz *grafo,int inicio);


double calcular_execucao(clock_t inicio, clock_t fim);


// funções

//lista adjascente
Grafo_lista *iniciarGrafoLista(int tam, int numArestas);
Vertice* criarVertice_lista(Grafo_lista *g,int vertice);
void insercao_aresta_lista(Grafo_lista *grafo, int origem, int destino);
void liberar_lista(Grafo_lista *grafo);
void imprimir_grafo_lista(Grafo_lista *grafo);

//matriz adjascente
Grafo_Matriz *iniciarGrafoMatriz(int numVertices);
void liberar_matriz(Grafo_Matriz *grafo, int tam);
void insercao_aresta_matriz(Grafo_Matriz *grafo, int origem, int destino);

//ler arquivo
int numero_vetores(char nome_arq[]);
int numero_arestas(char nome_arq[]);
double ler_inserir_lista(char nome_arq[], Grafo_lista *grafoAdj);
double ler_inserir_matriz(char nome_arq[],Grafo_Matriz *grafoMat);

//algoritimo
int comparar_int(const void *a, const void *b);
void quick_sort(int *vetor, int tam);
double mediana(int *vetor,int tam);

//estastisticas grafo lista
int grau_vertice_lista(Grafo_lista *grafo, int idx);
Estatisticas estatisticas_lista(Grafo_lista *grafo);
//estastisticas grafo matriz
int grau_vertice_matriz(Grafo_Matriz *grafo, int vertice);
Estatisticas estatisticas_matriz(Grafo_Matriz *grafo);


#endif