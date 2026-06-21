#include <time.h>
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

//item 4 (busca em porfundidade) - ERICk >>>>> falta salvar as informações num arquivo
void DFSVisita(Grafo_lista *g, int v, int *visitado, int *pai, int *nivel) {
    visitado[v] = 1;//coloca o vertice que o usuario escolheu como visitado ja

    Vertice *atual = g->listaAdj[v].prox; // vizinho de v
    while (atual != NULL) { // ate que chegue no nulo
        int u = atual->id; //guarda o identificador do vizinho atual
        if (!visitado[u]) { //aqui ele so vai descer se o vizinho nao tiver sido visitado
            pai[u] = v; //coloca v como pai de u
            nivel[u] = nivel[v] + 1; //nivel do filho um nivel a mais que o nivel do pai
            DFSVisita(g, u, visitado, pai, nivel); //recursao nos vizinhos até o null
        }
        atual = atual->prox; //vai pro proximo da lista
    }
}

void DFS(Grafo_lista *g, int verticeInicial, int *pai, int *nivel) {
    int n = g->numVertices; // guarda a quantidad de vertices 
    int *visitado = calloc(n, sizeof(int)); //aloca um vetor zerado do tamanho da quantidade de vertices

    for (int i = 0; i < n; i++) { //coloca o pai de todos os vertices como -1 pra ser um valor sentinela 
        pai[i] = -1;
        nivel[i] = -1;
    }
    nivel[verticeInicial] = 0; // raiz da arvore é zero

    clock_t inicio = clock(); // quantidades de ticks antes de chamar a funcao
    DFSVisita(g, verticeInicial, visitado, pai, nivel); //chama a funcao
    clock_t fim = clock(); // quantidade de ticks depois da funao finalziar

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC; // pega o final menos inicio e converte pra segundos pra ver quanto tempo demorou
    printf("Tempo de execucao da DFS: %f segundos\n", tempo);

    free(visitado); // libera o vetor de visitados
}
