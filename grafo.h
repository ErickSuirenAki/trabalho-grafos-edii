typedef struct Vertice{
	int id;
	struct Vertice *prox;
}Vertice;



typedef struct Grafo_lista{
    int numVertices;
    int numArestas;
    Vertice *listaAdj;
} Grafo_lista;


void DFSVisita(Grafo_lista *g, int v, int *visitado, int *pai, int *ordem, int *contador);

void DFS(Grafo_lista *g, int verticeInicial, int *pai, int *ordem);
