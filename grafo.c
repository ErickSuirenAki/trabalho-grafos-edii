#include <stdio.h>
#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>

Fila *criarFila(int tamanho){
    Fila *f = malloc(sizeof(Fila));
    f->dados = malloc(sizeof(int) * tamanho);
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = tamanho;
    return f;
}
void enfilerar(Fila *f,int vertice){
    f->dados[f->fim++] = vertice;
}
int desenfilerar(Fila *f){
    return f->dados[f->inicio++];
}
bool filaVazia(Fila *f){
    return f->inicio == f->fim;
}
ListaVertice BFS(Grafo_lista *grafo,int inicio){
    int tamanho = grafo->numVertices;
    int *visitados = calloc(tamanho,sizeof(int));

    Fila *f = criarFila(grafo->numVertices);
    ListaVertice vertices; vertices.tamanho = 0;
    vertices.idx =(int*) malloc(sizeof(int) * tamanho);


    visitados[inicio] = 1;
    enfilerar(f,inicio);
    
    int index = 0;
    while(!filaVazia(f)){
        int atual = desenfilerar(f);
        
        vertices.idx[index] = atual;
        index++;
        vertices.tamanho++;

        Vertice *vizinho = grafo->listaAdj[atual];
        while(vizinho != NULL){
            int v = vizinho->id;
           
            if(!(visitados[v])){
                visitados[v] = 1;
                enfilerar(f,v);
            }
            vizinho = vizinho->prox;
        }
    }
    free(visitados);
    free(f->dados);
    free(f);
    return vertices;
}





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
void imprimir_grafo_lista(Grafo_lista *grafo){
    for(int i = 0 ; i < grafo->numVertices ; i++){
        printf("[%d]->",i);
        Vertice *aux = grafo->listaAdj[i];
        while(aux != NULL){
            printf("%d->",aux->id);
            aux = aux->prox;
        }
        printf("*\n");
    }
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
        printf("ERRO, não foi possivel abrir o arquivo: %s",nome_arq);
        return;
    }else{
        int origem, destino,lixo;
        fscanf(file,"%d",&lixo);
        while(fscanf(file,"%d %d",&origem,&destino) == 2){
            insercao_aresta_matriz(grafoMat,origem,destino);
            insercao_aresta_lista(grafoAdj, origem, destino);
        }
        fclose(file);
    }
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


