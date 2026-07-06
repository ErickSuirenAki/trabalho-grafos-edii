#include <stdbool.h>
#include <time.h>

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
    int *dados;
    int topo;
    int tamanho;
}Pilha;

typedef struct {
    int tam;
    int numArestas;
    Vertice **listaAdj;
    Vertice *pool;
    int *graus;
    int numVertices;
    int index_pool;
} Grafo_lista;

typedef struct{
    int tam;
    int numArestas;
    bool **matriz;
    int *graus;
} Grafo_Matriz;

typedef struct{
    int numVertices;
    int numArestas;
    int grauMinimo;
    int grauMaximo;
    double grauMedio;
    double grauMediana;
    bool valido;
} Estatisticas;

typedef struct{
    int *caminho;
    int *pai;
    int *nivel;
    int tamanho;
    double tempo_execucao;
    bool valido;
} ResultadoBFS;

typedef struct{
    int *pai;
    int *nivel;
}ResultadoDFS;

typedef struct Componente{
    int tamanho;
    int *vertices;
} Componente;

typedef struct{
    Estatisticas status;
    Componente *comp;
    int numComp;
} EstudoCaso;

//Implementacao fila
Fila *criarFila(int tamanho);
void enfilerar(Fila *f,int vertice);
int desenfilerar(Fila *f);
bool filaVazia(Fila *f);

//Implementacao Pilha
Pilha *criarPilha(int capacidade);
int pilhaVazia(Pilha *p);
void empilhar(Pilha *p, int v);
int desempilhar(Pilha *p);
void liberarPilha(Pilha *p);


//metodos auxiliares gerais
int gerar_numero_aletorio(int inicio);
double calcular_execucao(clock_t inicio, clock_t fim);
//testes de execucao
double BFS_100(Grafo_Matriz *grafoMat,Grafo_lista *grafoLista,int tipoLista);
double DFS_100(Grafo_Matriz *gMatriz, Grafo_lista *gLista, int tipoGrafo);

double calcular_memoria_lista(long V, long E);
double calcular_memoria_matriz(int tam);

//Estudo caso
int distancia_pares_lista(Grafo_lista *g,int inicio,int index_pai);
int distancia_pares_matriz(Grafo_Matriz *g,int inicio,int index_pai);
int pai_vertices(int *pai,int vertice);

//Implementacao BFS
ResultadoBFS iniciar_resultado_BFS(int tam);
ResultadoBFS BFS_lista(Grafo_lista *grafo,int inicio);
ResultadoBFS BFS_matriz(Grafo_Matriz *grafo,int inicio);
void liberar_bfs(ResultadoBFS bfs);


//Implementacao Lista
Grafo_lista *iniciarGrafoLista(int tam, int numArestas);
Vertice* criarVertice_lista(Grafo_lista *g,int vertice);
void insercao_aresta_lista(Grafo_lista *grafo, int origem, int destino);
void liberar_lista(Grafo_lista *grafo);
void imprimir_grafo_lista(Grafo_lista *grafo);

//Implementacao Matriz
Grafo_Matriz *iniciarGrafoMatriz(int tam);
void liberar_matriz(Grafo_Matriz *grafo, int tam);
void insercao_aresta_matriz(Grafo_Matriz *grafo, int origem, int destino);

//dados arquivos
int numero_vetores(char nome_arq[]);
int numero_arestas(char nome_arq[]);
void ler_inserir_grafo(char nome_arq[],Grafo_Matriz *grafoMat, Grafo_lista *grafoAdj, int tipoGrafo);

//Algoritmo de ordenacao
int comparar_int(const void *a, const void *b);
void quick_sort(int *vetor, int tam);
double mediana(int *vetor,int tam);

//Implementacao Estisticas
Estatisticas estatisticas_lista(Grafo_lista *g);
Estatisticas estatisticas_matriz(Grafo_Matriz *g);
Estatisticas calcular_estatisticas_base(int *vetor_graus, int numVertices, int numArestas);

//Implementacao escrita arquivo saida
void escrever_arquivo(EstudoCaso e,char nomeArquivo[],int opcao);

//Implementacao DFS - Erick
void DFSLista(Grafo_lista *g, int v, int *visitado, int *listaVertices, int *tamanho);
void DFSMatriz(Grafo_Matriz *g, int v, int *visitado, int *listaVertices, int *tamanho);
void imprimirArvoreDFS(int *pai, int *nivel, int numVertices, const char *nomeArquivo);

//Implementacao Compontentes
int compararComponentes(const void *a, const void *b);
void DFS_ComponenteLista(Grafo_lista *g, int v, int *visitado, int *listaVertices, int *tamanho);
Componente* componentesConexasLista(Grafo_lista *g, int *numComponentes);
void DFS_ComponenteMatriz(Grafo_Matriz *g, int v, int *visitado, int *listaVertices, int *tamanho);
Componente* componentesConexasMatriz(Grafo_Matriz *g, int *numComponentes);
void escreverComponentes(const char *nomeArquivo, Componente *componentes, int numComponentes);

int distanciaLista(Grafo_lista *g, int origem, int destino);
int distanciaMatriz(Grafo_Matriz *g, int origem, int destino);

//Implementacao Diametro Grafo
int diametroLista(Grafo_lista *g);
int diametroMatriz(Grafo_Matriz *g);
int diametroAproximado(Grafo_lista *lista, Grafo_Matriz *matriz, int *pU, int *pV);
