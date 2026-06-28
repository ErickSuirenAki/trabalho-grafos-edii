#include <stdio.h>
#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include<time.h>

//fila Implementacao
Fila *criarFila(int tamanho){
    Fila *f = malloc(sizeof(Fila));
    f->dados = malloc(sizeof(int) * tamanho);
    if(f->dados == NULL){
        if(f != NULL)free(f);
        return NULL;
    }
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

//metodos auxiliares gerais
int gerar_numero_aletorio(int limite){
    return rand() % limite;
}
double calcular_execucao(clock_t inicio, clock_t fim){
    return ((double)(fim - inicio)); //tempo execucao ms
}
bool abrirArquivo(char caminho_arquivo[]){
    FILE *file = fopen(caminho_arquivo,"r");
    return (file != NULL);
}

//calcular memoria utilizada
double calcular_memoria_lista(long V, long E){
    long long arestas_armazenadas = 2LL * E;
    long long bytes = 
        sizeof(Grafo_lista) +                           
        (long long)V * sizeof(Vertice*) +              
        (long long)V * sizeof(int) +                   
        arestas_armazenadas * sizeof(Vertice);          

    double mb = (double)bytes / 1048576.0;
    return mb;
}
double calcular_memoria_matriz(int tam){
    long long bytes = 
        sizeof(Grafo_Matriz) +                          
        (long long)tam * sizeof(bool*) +                 
        (long long)tam * sizeof(int) +                 
        (long long)tam * tam * sizeof(bool);            

    double mb = (double)bytes / 1048576.0;
    return mb;
}

//BFS
ResultadoBFS iniciar_resultado_BFS(int tam){
    ResultadoBFS bfs;
    bfs.valido = true;
    bfs.caminho = malloc(sizeof(int) * tam);
    bfs.nivel = malloc(sizeof(int) * tam);
    bfs.pai = malloc(sizeof(int) * tam);

    if(bfs.caminho == NULL || bfs.nivel == NULL || bfs.pai == NULL){ //erro de execucao
        liberar_bfs(bfs);
        bfs.valido = false;
        return bfs;
    }

    for(int i = 0; i < tam ; i++){
        bfs.caminho[i] = -1;
        bfs.nivel[i] = -1;
        bfs.pai[i] = -1;
    }
    bfs.tamanho = 0;
    return bfs;
}
ResultadoBFS BFS_lista(Grafo_lista *grafo,int inicio){
    clock_t origem,fim; 

    origem = clock();

    int tamanho = grafo->tam;

    int *visitados = calloc(tamanho,sizeof(int));
    Fila *fila = criarFila(grafo->tam);
    ResultadoBFS vertices = iniciar_resultado_BFS(tamanho);

    if(fila == NULL || visitados == NULL || vertices.valido == false){ //erro de execucao
        if(visitados != NULL)free(visitados);
        vertices.valido = false;
        return vertices;
    }

    vertices.nivel[inicio] = 0;
    visitados[inicio] = 1;
    enfilerar(fila,inicio);
    
    int index = 0;
    while(!filaVazia(fila)){
        int atual = desenfilerar(fila);
        
        vertices.caminho[index] = atual;
        index++;
        vertices.tamanho++;

        Vertice *vizinho = grafo->listaAdj[atual];
        while(vizinho != NULL){
            int v = vizinho->id;
           
            if(!(visitados[v])){
                visitados[v] = 1;
                vertices.pai[v] = atual;
                vertices.nivel[v] = vertices.nivel[atual] + 1;
                enfilerar(fila,v);
            }
            vizinho = vizinho->prox;
        }
    }
    free(visitados);
    free(fila->dados);
    free(fila);
    
    fim = clock();
    vertices.tempo_execucao = calcular_execucao(origem,fim);
    
    return vertices;
}
ResultadoBFS BFS_matriz(Grafo_Matriz *grafo,int inicio){
    clock_t origem,fim;

    origem = clock();
    
    int tamanho = grafo->tam;

    //inicializa as variaveis e fila
    int *visitado = calloc(tamanho,sizeof(int));
    Fila *fila = criarFila(tamanho);
    ResultadoBFS vertices = iniciar_resultado_BFS(tamanho);

    if(fila == NULL || visitado == NULL || vertices.valido == false){ //erro de execucao
        if(visitado != NULL)free(visitado);
        vertices.valido = false;
        return vertices;
    }
   

    vertices.nivel[inicio] = 0;
    visitado[inicio] = 1;
    enfilerar(fila,inicio);

    int index = 0;
    while(!filaVazia(fila)){
        int atual = desenfilerar(fila);
        
        vertices.caminho[index] = atual; //adiciona o elemento no vetor
        index++; vertices.tamanho++;

        for(int i = 0 ; i < tamanho ; i++){
            if(grafo->matriz[atual][i] && !visitado[i]){
                visitado[i] = 1;
                vertices.pai[i] = atual;
                vertices.nivel[i] = vertices.nivel[atual] + 1;
                enfilerar(fila,i);
            }
        }
    }
    free(visitado);
    free(fila->dados);
    free(fila);

    fim = clock();
    vertices.tempo_execucao = calcular_execucao(origem,fim);
    return vertices;
}
void liberar_bfs(ResultadoBFS bfs){
    if(bfs.caminho != NULL) free(bfs.caminho);
    if(bfs.nivel   != NULL) free(bfs.nivel);
    if(bfs.pai     != NULL) free(bfs.pai);
}
//operacoes que usam BFS
int distancia_pares_lista(Grafo_lista *g,int inicio,int index_pai){
    ResultadoBFS bfs = BFS_lista(g,inicio);
    
    if(!bfs.valido) return -2; //erro de execucao

    int distancia = bfs.nivel[index_pai];
    liberar_bfs(bfs);
    return distancia;
}
int distancia_pares_matriz(Grafo_Matriz *g,int inicio,int index_pai){
    ResultadoBFS bfs = BFS_matriz(g,inicio);

    if(!bfs.valido) return -2; //erro de execucao
    
    int distancia = bfs.nivel[index_pai];
    liberar_bfs(bfs);
    return distancia;
}
bool pai_vertices_lista(Grafo_lista *g, EstudoCaso *estudo){
    ResultadoBFS bfs1 = BFS_lista(g, 1);
    ResultadoBFS bfs2 = BFS_lista(g, 2);
    ResultadoBFS bfs3 = BFS_lista(g, 3);

    if(!bfs1.valido || !bfs2.valido || !bfs3.valido) return false; //erro de execucao

    estudo->pai[0][0] = bfs1.pai[10];
    estudo->pai[0][1] = bfs1.pai[20];
    estudo->pai[0][2] = bfs1.pai[30];

    estudo->pai[1][0] = bfs2.pai[10];
    estudo->pai[1][1] = bfs2.pai[20];
    estudo->pai[1][2] = bfs2.pai[30];

    estudo->pai[2][0] = bfs3.pai[10];
    estudo->pai[2][1] = bfs3.pai[20];
    estudo->pai[2][2] = bfs3.pai[30];

    liberar_bfs(bfs1);
    liberar_bfs(bfs2);
    liberar_bfs(bfs3);
    return true;
}
bool pai_vertices_matriz(Grafo_Matriz *g, EstudoCaso *estudo){
    ResultadoBFS bfs1 = BFS_matriz(g, 1);
    ResultadoBFS bfs2 = BFS_matriz(g, 2);
    ResultadoBFS bfs3 = BFS_matriz(g, 3);

    if(!bfs1.valido || !bfs2.valido || !bfs3.valido) return false; //erro de execucao
    
    estudo->pai[0][0] = bfs1.pai[10];
    estudo->pai[0][1] = bfs1.pai[20];
    estudo->pai[0][2] = bfs1.pai[30];

    estudo->pai[1][0] = bfs2.pai[10];
    estudo->pai[1][1] = bfs2.pai[20];
    estudo->pai[1][2] = bfs2.pai[30];

    estudo->pai[2][0] = bfs3.pai[10];
    estudo->pai[2][1] = bfs3.pai[20];
    estudo->pai[2][2] = bfs3.pai[30];

    liberar_bfs(bfs1);
    liberar_bfs(bfs2);
    liberar_bfs(bfs3);
}



//lista Implementacao
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

//matriz Implementacao
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
    grafo->matriz[origem][destino] = true;
    grafo->matriz[destino][origem] = true;
    grafo->numArestas++;
    grafo->graus[origem]++;
    grafo->graus[destino]++;
}

//dados arquivos
int numero_vetores(char nome_arq[]){
    FILE *file = fopen(nome_arq,"r");
    if(file == NULL){ //erro de execucao
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
    if(file == NULL){ //erro de execucao
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
    return tempo_execucao; 
}
double ler_inserir_matriz(char nome_arq[],Grafo_Matriz *grafoMat){
    FILE *file = fopen(nome_arq,"r");
    
    double tempo_execucao = 0.0;
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
    return tempo_execucao;
}

//algoritimo de ordenacao
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
    est.valido = true;
    int minGrau = -1;
    int maxGrau = -1;
    int grau,somaGrau=0;
    int numVertices = g->tam - 1;
   
    int *copia_graus = malloc(numVertices * sizeof(int));
    if(copia_graus == NULL){ //error de execucao
        est.valido = false;
        return est;
    }

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
    est.valido = true;
    int minGrau = -1;
    int maxGrau = -1;
    int grau,somaGrau=0;
    int numVertices = grafo->tam - 1;
   
    int *copia_graus = malloc(numVertices * sizeof(int));
    if(copia_graus == NULL){
        est.valido = false;
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

//arquivo estudo de casos
bool criar_arquivo_estudo(char nome_arq[]){
    FILE *teste = fopen(nome_arq,"w");
    if(teste != NULL){
        fclose(teste);
        return true;
    }
    return false;
}
void escrever_arquivo(EstudoCaso e,char nome_arq[],char tipo_grafo[]){
    FILE *file = fopen(nome_arq,"a");
    if(file == NULL){
        printf("\nERRO: nao foi possivel criar um arquivo");
        return;
    }
    fprintf(file,"---------- %s ----------\n",tipo_grafo);
    fprintf(file,"==== %s ====\n",nome_arq);
    fprintf(file,"Estatisticas da lista:\n");
    fprintf(file,"Memoria utilizada: %.2f mb\n",e.memoria_utilizada);
    fprintf(file, "Numero de vertices: %d\n", e.status.numVertices);
    fprintf(file, "Numero de arestas: %d\n", e.status.numArestas);
    fprintf(file, "Grau minimo: %d\n", e.status.grauMinimo);
    fprintf(file, "Grau maximo: %d\n", e.status.grauMaximo);
    fprintf(file, "Grau medio: %.2f\n", e.status.grauMedio);
    fprintf(file, "Grau mediana: %.2f\n", e.status.grauMediana);
    fprintf(file,"Tempo medio de execucao em 100 BFS: %.2fms\n",e.tempo_medio_BFS);
    
    int vertices[] = {10,20,30};
    for(int i = 0 ; i < 3 ; i++){
        fprintf(file,"BFS inicio: %d\n",i+1);
        for(int j = 0; j < 3 ; j++){
            fprintf(file,"Pai(%d): %d\n",vertices[j],e.pai[i][j]);
        }
    }
    fprintf(file,"Distancia entre pares(10,20): %d\n",e.distancia_pares[0]);
    fprintf(file,"Distancia entre pares(10,30): %d\n",e.distancia_pares[1]);
    fprintf(file,"Distancia entre pares(20,30): %d\n",e.distancia_pares[2]);
    fprintf(file,"==== fim da execucao ====\n");
    char somente_nome[50];
    sscanf(nome_arq, "%*[^/]/%s",somente_nome);
    printf("Criado estudo da (%s) no diretorio /estudoCaso com nome: %s\n",tipo_grafo,somente_nome);
}


