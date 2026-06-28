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
    bool valido;
}Estatisticas;

typedef struct{
    int *caminho;
    int *pai;
    int *nivel;
    int tamanho;
    double tempo_execucao;
    bool valido;
}ResultadoBFS;

typedef struct 
{
    Estatisticas status;
    double tempo_medio_BFS;
    int pai[3][3];
    int distancia_pares[3];
    double memoria_utilizada;
    bool valido;
}EstudoCaso;




//fila Implementacao
Fila *criarFila(int tamanho);
void enfilerar(Fila *f,int vertice);
int desenfilerar(Fila *f);
bool filaVazia(Fila *f);

//metodos auxiliares gerais
int gerar_numero_aletorio(int inicio);
double calcular_execucao(clock_t inicio, clock_t fim);
bool abrirArquivo(char caminho_arquivo[]);

//calcular memoria utilizada
double calcular_memoria_lista(long V, long E);
double calcular_memoria_matriz(int tam);

//BFS
ResultadoBFS iniciar_resultado_BFS(int tam);
ResultadoBFS BFS_lista(Grafo_lista *grafo,int inicio);
ResultadoBFS BFS_matriz(Grafo_Matriz *grafo,int inicio);
void liberar_bfs(ResultadoBFS bfs);
//operacoes que usam BFS
int distancia_pares_lista(Grafo_lista *g,int inicio,int index_pai);
int distancia_pares_matriz(Grafo_Matriz *g,int inicio,int index_pai);

bool pai_vertices_lista(Grafo_lista *g, EstudoCaso *estudo);
bool pai_vertices_matriz(Grafo_Matriz *g, EstudoCaso *estudo);



//lista Implementacao
Grafo_lista *iniciarGrafoLista(int tam, int numArestas);
Vertice* criarVertice_lista(Grafo_lista *g,int vertice);
void insercao_aresta_lista(Grafo_lista *grafo, int origem, int destino);
void liberar_lista(Grafo_lista *grafo);
void imprimir_grafo_lista(Grafo_lista *grafo);

//matriz Implementacao
Grafo_Matriz *iniciarGrafoMatriz(int numVertices);
void liberar_matriz(Grafo_Matriz *grafo, int tam);
void insercao_aresta_matriz(Grafo_Matriz *grafo, int origem, int destino);

//dados arquivos
int numero_vetores(char nome_arq[]);
int numero_arestas(char nome_arq[]);
double ler_inserir_lista(char nome_arq[], Grafo_lista *grafoAdj);
double ler_inserir_matriz(char nome_arq[],Grafo_Matriz *grafoMat);

//algoritimo de ordenacao
int comparar_int(const void *a, const void *b);
void quick_sort(int *vetor, int tam);
double mediana(int *vetor,int tam);

//estastisticas grafo lista
int grau_vertice_lista(Grafo_lista *grafo, int idx);
Estatisticas estatisticas_lista(Grafo_lista *grafo);

//estastisticas grafo matriz
int grau_vertice_matriz(Grafo_Matriz *grafo, int vertice);
Estatisticas estatisticas_matriz(Grafo_Matriz *grafo);

//arquivo estudo de casos
bool criar_arquivo_estudo(char nome_arq[]);
void escrever_arquivo(EstudoCaso e,char nome_arq[],char tipo_grafo[]);







#endif