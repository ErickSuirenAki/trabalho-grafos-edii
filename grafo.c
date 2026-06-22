#include <time.h>
#include <stdio.h>
#include "grafo.h"
#include <stdlib.h>

Grafo_lista *iniciarGrafoLista(int numVertices){

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
void inserirInicio(Grafo_lista *grafo, Vertice *vertice,int index){
   vertice->prox = grafo->listaAdj[index];
   grafo->listaAdj[index] = vertice;
}
void insercao_aresta_lista(Grafo_lista *grafo, int origem, int destino){
	Vertice *novoVertice = criarVertice_lista(destino);
    Vertice *simetricoVertice = criarVertice_lista(origem);
    
	if (novoVertice == NULL || simetricoVertice == NULL) {
        printf("Erro: sem memória para inserir aresta (%d, %d)\n", origem, destino);
        free(novoVertice);       
        free(simetricoVertice);
        return;
    }
    inserirInicio(grafo, novoVertice, origem);
    inserirInicio(grafo, simetricoVertice, destino);
    grafo->numArestas++;
}
void liberar_lista(Grafo_lista *grafo){
    for(int i = 0 ; i < grafo->numVertices ; i++){
        Vertice *atual = grafo->listaAdj[i];
        while(atual != NULL){
            Vertice *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }
    free(grafo->listaAdj);
    free(grafo);
}


Grafo_Matriz *iniciarGrafoMatriz(int numVertices){
    Grafo_Matriz *grafo = malloc(sizeof(Grafo_Matriz));
    if(grafo == NULL) return NULL;

    grafo->matriz = calloc(numVertices, sizeof(int *));

    if (grafo->matriz == NULL){
        free(grafo);
        return NULL;
    }
        
    int i;

    for(i=0 ; i < numVertices ; i++){
        grafo->matriz[i] = calloc(numVertices,sizeof(int));
        
        if(grafo->matriz[i] == NULL){
            liberar_matriz(grafo,i);
            return NULL;
        }    
    }
    grafo->numArestas = 0;
    grafo->numVertices = numVertices;
    return grafo;
}
void liberar_matriz(Grafo_Matriz *grafo, int tam){
    for(int i = 0; i < tam; i++){
        free(grafo->matriz[i]);
    }
    free(grafo->matriz);
    free(grafo);
}
void insercao_aresta_matriz(Grafo_Matriz *grafo, int origem, int destino){
    if(grafo->matriz[origem][destino] == 0){
        grafo->matriz[origem][destino] = 1;
        grafo->matriz[destino][origem] = 1;
        grafo->numArestas++;
    }
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
void ler_inserir(char nome_arq[], Grafo_lista *grafoAdj, Grafo_Matriz *grafoMat){
    FILE *file = fopen(nome_arq,"r");

    if(file == NULL){
        printf("ERRO ao abrir arquivo\n");
        return;
    }

    int origem, destino, lixo;
    fscanf(file,"%d",&lixo);

    while(fscanf(file,"%d %d",&origem,&destino) == 2){

        if (grafoMat != NULL) {
            insercao_aresta_matriz(grafoMat, origem-1, destino-1);
        }

        if (grafoAdj != NULL) {
            insercao_aresta_lista(grafoAdj, origem-1, destino-1);
        }
    }

    fclose(file);
}


int comp(const void * a, const void *b){
    return (*(int *)a - *(int *)b);
}
double mediana(int vetor[],int tam){
    qsort(vetor,tam,sizeof(int),comp);
     if (tam % 2 == 1) {
       return vetor[tam / 2];
    } else {
        return (double)((vetor[tam / 2 - 1] +  vetor[tam / 2])) / 2.0;
    }
}


int grau_vertice_lista(Grafo_lista *grafo, int n){
    Vertice *aux = grafo->listaAdj[n];
    int grau = 0;
    while(aux != NULL){
        aux = aux->prox;
        grau++;
    }
    return grau;
}
Estatisticas estatisticas_lista(Grafo_lista *grafo){
    Estatisticas est;

    int grau;
    int somaGrau = 0;
    int TAM = grafo->numVertices;

    int vertices_grau[TAM];
    for(int i = 0 ; i < TAM ; i++){
        grau = grau_vertice_lista(grafo,i);
        vertices_grau[i] = grau;
        somaGrau += grau;
    }

    est.grauMediana = mediana(vertices_grau,TAM);
    est.grauMinimo = vertices_grau[0];
    est.grauMedio = (double) somaGrau / (double) TAM;
    est.grauMaximo = vertices_grau[TAM-1];
    est.numVertices = grafo->numVertices;
    est.numArestas = grafo->numArestas;
    return est;
}


int grau_vertice_matriz(Grafo_Matriz *grafo, int vertice){
    int grau = 0;
    for(int j = 0; j < grafo->numVertices ; j++){
        if(grafo->matriz[vertice][j] == 1){
            grau++;
        } 
    }
    return grau;
}
Estatisticas estatisticas_matriz(Grafo_Matriz *grafo){
    Estatisticas est;

    int grau;
    int somaGrau = 0;
    int tam = grafo->numVertices;
    
    int vertices_grau[tam];
    for(int i = 0; i < tam ; i++){
        grau = grau_vertice_matriz(grafo,i);
        vertices_grau[i] = grau;
        somaGrau += grau;
    }
    est.grauMediana = mediana(vertices_grau,tam);
    est.grauMinimo = vertices_grau[0];
    est.grauMedio = (double) somaGrau / (double) tam;
    est.grauMaximo = vertices_grau[tam-1];
    est.numVertices = grafo->numVertices;
    est.numArestas = grafo->numArestas;
    return est;
}

//item 4 (busca em porfundidade) - ERICk
void DFSVisitaLista(Grafo_lista *g, int v, int *visitado, int *pai, int *nivel) {
    visitado[v] = 1;//coloca o vertice que o usuario escolheu como visitado ja

    Vertice *atual = g->listaAdj[v]; // vizinho de v
    while (atual != NULL) { // ate que chegue no nulo
        int u = atual->id; //guarda o identificador do vizinho atual
        if (!visitado[u]) { //aqui ele so vai descer se o vizinho nao tiver sido visitado
            pai[u] = v; //coloca v como pai de u
            nivel[u] = nivel[v] + 1; //nivel do filho um nivel a mais que o nivel do pai
            DFSVisitaLista(g, u, visitado, pai, nivel); //recursao nos vizinhos até o null
        }
        atual = atual->prox; //vai pro proximo da lista
    }
}

void DFSLista(Grafo_lista *g, int verticeInicial, int *pai, int *nivel) {
    int n = g->numVertices; // guarda a quantidad de vertices 
    int *visitado = calloc(n, sizeof(int)); //aloca um vetor zerado do tamanho da quantidade de vertices

    for (int i = 0; i < n; i++) { //coloca o pai de todos os vertices como -1 pra ser um valor sentinela 
        pai[i] = -1;
        nivel[i] = -1;
    }
    nivel[verticeInicial] = 0; // raiz da arvore é zero
    pai[verticeInicial]= -1;
    DFSVisitaLista(g, verticeInicial, visitado, pai, nivel); //chama a funcao
    free(visitado); // libera o vetor de visitados
}

void DFSVisitaMatriz(Grafo_Matriz *g, int v, int *visitado, int *pai, int *nivel){
    visitado[v] = 1;//coloca o vertice que o usuario escolheu como visitado ja

    for(int u = 0; u < g->numVertices; u++){
        if(g->matriz[v][u] ==1 && !visitado[u]){
            pai[u] = v;
            nivel[u] = nivel[v] + 1;
            DFSVisitaMatriz(g, u,visitado, pai, nivel);
        }
    }
}

void DFSMatriz(Grafo_Matriz *g, int verticeInicial, int *pai, int *nivel){
    int n = g->numVertices;

    int *visitado= calloc(n, sizeof(int));

    for(int i = 0; i < n; i++){
        pai[i] = -1;
        nivel[i] = -1;
    }
    nivel[verticeInicial] = 0;
    pai[verticeInicial] = -1;
    DFSVisitaMatriz(g, verticeInicial, visitado, pai, nivel);
    free(visitado);
}

void imprimirArvoreDFS(int *pai, int *nivel, int numVertices, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de saida.\n");
        return;
    }

    fprintf(arquivo, "Vertice Pai Nivel\n");
    for (int i = 0; i < numVertices; i++) {
        fprintf(arquivo, "%d %d %d\n", i, pai[i], nivel[i]);
    }

    fclose(arquivo);
}



//item 6 (componentes conexos) - Erick
int compararComponentes(const void *a, const void *b) {
    Componente *compA = (Componente *)a;
    Componente *compB = (Componente *)b;
    return compB->tamanho - compA->tamanho;
}

void DFS_ComponenteLista(Grafo_lista *g, int v, int *visitado, int *listaVertices, int *tamanho) {
    visitado[v] = 1;
    listaVertices[(*tamanho)++] = v;
    Vertice *atual = g->listaAdj[v];
    while (atual != NULL) {
        int u = atual->id;
        if (!visitado[u]) {
            DFS_ComponenteLista(g, u, visitado, listaVertices, tamanho);
        }
        atual = atual->prox;
    }
}

Componente* componentesConexasLista(Grafo_lista *g, int *numComponentes) {
    int n = g->numVertices;
    int *visitado = calloc(n, sizeof(int));
    Componente *componentes = malloc(n * sizeof(Componente));
    int contador = 0;
    for (int i = 0; i < n; i++) {
        if (!visitado[i]) {
            int *listaTemp = malloc(n * sizeof(int));
            int tamanho = 0;

            DFS_ComponenteLista(g, i, visitado, listaTemp, &tamanho);
            int *listaFinal = malloc(tamanho * sizeof(int));
            for (int j = 0; j < tamanho; j++) {
                listaFinal[j] = listaTemp[j];
            }
            free(listaTemp);
            componentes[contador].tamanho = tamanho;
            componentes[contador].vertices = listaFinal;
            contador++;
        }
    }
   Componente *tmp = realloc(componentes, contador * sizeof(Componente));
    if (tmp != NULL) {
        componentes = tmp;
    }   
    *numComponentes = contador;
    free(visitado);
    qsort(componentes, contador, sizeof(Componente), compararComponentes);
    return componentes;
}


void DFS_ComponenteMatriz(Grafo_Matriz *g,int v,int *visitado, int *listaVertices, int *tamanho){
    visitado[v] = 1;
    listaVertices[(*tamanho)++] = v;
    for(int u = 0; u < g->numVertices; u++){
        if(g->matriz[v][u] == 1 && !visitado[u]){
            DFS_ComponenteMatriz(g, u,visitado, listaVertices, tamanho);
}

}}

Componente* componentesConexasMatriz(Grafo_Matriz *g, int *numComponentes){
    int n = g->numVertices;
    int *visitado = calloc(n, sizeof(int));
    Componente *componentes = malloc(n * sizeof(Componente));
    int contador = 0;
    for(int i = 0; i < n; i++){

        if(!visitado[i]){
            int *listaTemp = malloc(n * sizeof(int));
            int tamanho = 0;
            DFS_ComponenteMatriz(g, i, visitado, listaTemp, &tamanho );

            int *listaFinal = malloc(tamanho * sizeof(int));

            for(int j = 0; j < tamanho; j++){
                listaFinal[j] = listaTemp[j];
            }
            free(listaTemp);
            componentes[contador].tamanho = tamanho;
            componentes[contador].vertices = listaFinal;

            contador++;
        }
    }

    Componente *tmp = realloc(componentes, contador * sizeof(Componente));
    if (tmp != NULL) {
        componentes = tmp;
    }
    *numComponentes = contador;
    free(visitado);
    qsort(componentes, contador, sizeof(Componente), compararComponentes);
    return componentes;
}


void escreverComponentes(const char *nomeArquivo, Componente *componentes, int numComponentes) {
    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de saida.\n");
        return;
    }

    fprintf(arquivo, "Numero de componentes: %d\n\n", numComponentes);

    for (int i = 0; i < numComponentes; i++) {
        fprintf(arquivo, "Componente %d\n", i + 1);
        fprintf(arquivo, "Tamanho: %d\n", componentes[i].tamanho);
        fprintf(arquivo, "Vertices: ");

        for (int j = 0; j < componentes[i].tamanho; j++) {
            fprintf(arquivo, "%d ", componentes[i].vertices[j]);
        }

        fprintf(arquivo, "\n\n");
    }

    fclose(arquivo);
}