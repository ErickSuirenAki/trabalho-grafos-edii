
#ifndef GRAFO_H
#define GRAFO_H

typedef struct Vertice {
    int id;
    struct Vertice *prox;
} Vertice;
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

typedef struct Componente {
    int tamanho;
    int *vertices;
} Componente;

// funções

//lista adjascente
Grafo_lista *iniciarGrafoLista(int numVertices);
Vertice* criarVertice_lista(int idt);
void inserirInicio(Grafo_lista *grafo, Vertice *vertice,int index);
void insercao_aresta_lista(Grafo_lista *grafo, int origem, int destino);
void liberar_lista(Grafo_lista *grafo);

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


//DFS
void DFSVisitaLista(Grafo_lista *g, int v, int *visitado, int *pai, int *nivel);

void DFSLista(Grafo_lista *g, int verticeInicial, int *pai, int *ordem);

void DFSVisitaMatriz(Grafo_Matriz *g, int v, int *visitado, int *pai, int *nivel);

void DFSMatriz(Grafo_Matriz *g, int verticeInicial, int *pai, int *nivel);

void imprimirArvoreDFS(int *pai, int *nivel, int numVertices, const char *nomeArquivo);

//Componentes conexas
int compararComponentes(const void *a, const void *b);

void DFS_ComponenteLista(Grafo_lista *g, int v, int *visitado, int *listaVertices, int *tamanho);

Componente* componentesConexasLista(Grafo_lista *g, int *numComponentes);

void DFS_ComponenteMatriz(Grafo_Matriz *g,int v,int *visitado, int *listaVertices, int *tamanho);

Componente* componentesConexasMatriz(Grafo_Matriz *g, int *numComponentes);

void escreverComponentes(const char *nomeArquivo, Componente *componentes, int numComponentes);


#endif