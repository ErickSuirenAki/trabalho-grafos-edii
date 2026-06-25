#include <stdio.h>
#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include<time.h>

//fila
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
double calcular_execucao(clock_t inicio, clock_t fim){
    return ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;
}



//BFS
ResultadoBFS BFS_lista(Grafo_lista *grafo,int inicio){
    
    int tamanho = grafo->tam;
    int *visitados = calloc(tamanho,sizeof(int));

    Fila *f = criarFila(grafo->tam);
    ResultadoBFS vertices; vertices.tamanho = 0;
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
ResultadoBFS BFS_matriz(Grafo_Matriz *grafo,int inicio){
    int tamanho = grafo->tam;
    int *visitado = calloc(tamanho,sizeof(int));

    Fila *fila = criarFila(grafo->tam);
    ResultadoBFS vertices; vertices.tamanho = 0;
    vertices.idx =(int*) malloc(sizeof(int) * tamanho);

    visitado[inicio] = 1;
    enfilerar(fila,inicio);

    int index = 0;
    while(!filaVazia(fila)){
        int atual = desenfilerar(fila);
        
        vertices.idx[index] = atual;
        index++; vertices.tamanho++;

        for(int i = 0 ; i < tamanho ; i++){
            if(grafo->matriz[atual][i] && !visitado[i]){
                visitado[i] = 1;
                enfilerar(fila,i);
            }
        }
    }
    free(visitado);
    free(fila->dados);
    free(fila);

    return vertices;

}




//lista
Grafo_lista *iniciarGrafoLista(int tam, int numArestas){
    Grafo_lista *grafo = malloc(sizeof(Grafo_lista));
    if(grafo == NULL) return NULL;
    
    grafo->listaAdj = calloc(tam, sizeof(Vertice*));
    
    if(grafo->listaAdj == NULL) {
        free(grafo);
        return NULL;
    }
    grafo->graus = calloc(tam,sizeof(int));
    grafo->pool = malloc(sizeof(Vertice) * 2 * numArestas);
    grafo->index_pool = 0;
    grafo->tam = tam;
    grafo->numArestas = 0;
    
    return grafo;
}
Vertice* criarVertice_lista(Grafo_lista *g,int vertice){
	Vertice *v = &g->pool[g->index_pool++];
    v->id = vertice;
    v->prox = NULL;
    return v;
}
void insercao_aresta_lista(Grafo_lista *g, int origem, int destino){
	Vertice *v1 = criarVertice_lista(g,destino);
    v1->prox = g->listaAdj[origem];
    g->listaAdj[origem] = v1;
     
    Vertice *v2 = criarVertice_lista(g,origem);
    v2->prox = g->listaAdj[destino];
    g->listaAdj[destino] = v2;
    g->graus[origem]++;
    g->graus[destino]++;
    g->numArestas++;
}
void liberar_lista(Grafo_lista *g){
    if (g == NULL) return;
    free(g->graus);
    free(g->listaAdj);  
    free(g->pool);      
    free(g);           
}
void imprimir_grafo_lista(Grafo_lista *grafo){
    for(int i = 0 ; i < grafo->tam ; i++){
        printf("[%d]->",i);
        Vertice *aux = grafo->listaAdj[i];
        while(aux != NULL){
            printf("%d->",aux->id);
            aux = aux->prox;
        }
        printf("*\n");
    }
}

//matriz
Grafo_Matriz *iniciarGrafoMatriz(int tam){
    if(tam > 50000) return NULL; //ta maluco, vai acabar com a ram
    
    Grafo_Matriz *grafo = calloc(1,sizeof(Grafo_Matriz));
    if(grafo == NULL) return NULL;

    grafo->matriz = calloc(tam, sizeof(bool*));

    if (grafo->matriz == NULL){
        free(grafo);
        return NULL;
    }
        
    int i;

    for(i=0 ; i < tam ; i++){
        grafo->matriz[i] = calloc(tam,sizeof(bool));
        
        if(grafo->matriz[i] == NULL){
            liberar_matriz(grafo,i);
            return NULL;
        }    
    }

    grafo->graus = calloc(tam,sizeof(int));
    if (grafo->graus == NULL){
        liberar_matriz(grafo, tam);
        return NULL;
    }   
    grafo->numArestas = 0;
    grafo->tam = tam;
    return grafo;
}
void liberar_matriz(Grafo_Matriz *grafo, int tam){
    if(grafo == NULL) return;
    if (grafo->matriz != NULL){
        for(int i = 0; i < tam; i++){
            free(grafo->matriz[i]);
        }
        free(grafo->matriz);
    }
    free(grafo->graus);
    free(grafo);
}
void insercao_aresta_matriz(Grafo_Matriz *grafo, int origem, int destino){
    grafo->graus[origem]++;
    grafo->graus[destino]++;
    if(grafo->matriz[origem][destino] == false){
        grafo->matriz[origem][destino] = true;
        grafo->matriz[destino][origem] = true;
        grafo->numArestas++;
    }
}

//arquivos
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
int numero_arestas(char nome_arq[]){
    FILE *file = fopen(nome_arq,"r");
    if(file == NULL){
        printf("ERRO, não foi possivel abrir o arquivo: %s",nome_arq);
        return -1;
    }else{
        int origem, destino,lixo,cont = 0;
        fscanf(file,"%d",&lixo);
        while(fscanf(file,"%d %d",&origem,&destino) == 2) 
            cont++;
        
        return cont;
    }   
}
double ler_inserir_lista(char nome_arq[], Grafo_lista *grafoAdj){
    FILE *file = fopen(nome_arq,"r");
    double tempo_execucao = 0.0;
    if(file == NULL){
        printf("ERRO, não foi possivel abrir o arquivo: %s",nome_arq);
        return 0.0;
    }else{
        int origem, destino,lixo;
        fscanf(file,"%d",&lixo);
        clock_t inicio,fim;
        while(fscanf(file,"%d %d",&origem,&destino) == 2){
            inicio = clock();
            insercao_aresta_lista(grafoAdj, origem, destino);
            fim = clock();
            tempo_execucao += calcular_execucao(inicio,fim);
        }
        fclose(file);
    }
    return tempo_execucao; 
}
double ler_inserir_matriz(char nome_arq[],Grafo_Matriz *grafoMat){
    FILE *file = fopen(nome_arq,"r");
    double tempo_execucao = 0.0;
    if(file == NULL){
        printf("ERRO, não foi possivel abrir o arquivo: %s",nome_arq);
    }else{
        int origem, destino,lixo;
        clock_t inicio,fim;

        fscanf(file,"%d",&lixo);

        while(fscanf(file,"%d %d",&origem,&destino) == 2){
            inicio = clock();
            insercao_aresta_matriz(grafoMat, origem, destino);
            fim = clock();
            tempo_execucao += ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;
        }   
        
        fclose(file);
    }
    return tempo_execucao;
}

//algoritimo
int comparar_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}
void quick_sort(int *vetor, int tam){
    qsort(vetor,tam,sizeof(int),comparar_int);
}
double mediana(int *vetor,int tam){
    quick_sort(vetor,tam);
    if(tam % 2 != 0){
        return (double) vetor[tam/2];
    }else{
        return (double)(vetor[tam/2 -1] + vetor[tam/2]) / 2.0;
    }
}

//estatisticas lista
int grau_vertice_lista(Grafo_lista *grafo, int idx){
    return grafo->graus[idx];
}
Estatisticas estatisticas_lista(Grafo_lista *g){
    Estatisticas est;
    int minGrau = -1;
    int maxGrau = -1;
    int grau,somaGrau=0;
    int numVertices = g->tam - 1;
   
    int *copia_graus = malloc(numVertices * sizeof(int));

    for(int i = 0; i < numVertices ; i++){
        grau = grau_vertice_lista(g, i + 1);
        copia_graus[i] = grau;
        somaGrau += grau;
        if(i == 0){
            maxGrau = grau;
            minGrau = grau;
        }else{
            if(grau > maxGrau) maxGrau = grau;
            if(grau < minGrau) minGrau = grau;
        }
    }
   
    
    
    est.grauMinimo = minGrau;
    est.grauMaximo = maxGrau;
    est.grauMedio  = (double)somaGrau / (double) numVertices;
    est.grauMediana = mediana(copia_graus,numVertices);
    est.numArestas = g->numArestas;
    est.numVertices= numVertices;

    free(copia_graus);
    return est;
}

//estatisticas matriz
int grau_vertice_matriz(Grafo_Matriz *grafo, int idx){
   return grafo->graus[idx];
}
Estatisticas estatisticas_matriz(Grafo_Matriz *grafo){
    Estatisticas est;
    int minGrau = -1;
    int maxGrau = -1;
    int grau,somaGrau=0;
    int numVertices = grafo->tam - 1;
   
    int *copia_graus = malloc(numVertices * sizeof(int));
    if(copia_graus == NULL){
        return est;
    }

    int i;
    for(i = 0; i < numVertices ; i++){
        grau = grau_vertice_matriz(grafo, i + 1);
        copia_graus[i] = grau;
        somaGrau += grau;
        if(i == 0){
            maxGrau = grau;
            minGrau = grau;
        }else{
            if(grau > maxGrau) maxGrau = grau;
            if(grau < minGrau) minGrau = grau;
        }
    }

    est.grauMaximo = maxGrau;
    est.grauMedio  = (double)somaGrau / (double) numVertices;
    est.grauMediana = mediana(copia_graus, numVertices);
    est.grauMinimo = minGrau;
    est.numArestas = grafo->numArestas;
    est.numVertices= numVertices;
    
    free(copia_graus);
    return est;
}


