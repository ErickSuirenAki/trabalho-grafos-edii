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
void liberar_fila(Fila *f){
    if(f->dados != NULL) free(f->dados);
    if(f != NULL)        free(f);
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
    return (double)(1000.0 * (fim - inicio) / CLOCKS_PER_SEC); //tempo execucao ms
}
double BFS_100(Grafo_Matriz *grafoMat,Grafo_lista *grafoLista,int tipoGrafo){
    int inicio;
    int tam = (tipoGrafo == 1) ? grafoLista->tam : grafoMat->tam;

    ResultadoBFS bfs;
    double soma_tempo = 0.0;
    //gera um numero aleatorio dentro do intervalo, faz o bfs e faz a media das 100 execucoes
    for(int cont = 0; cont < 100 ; cont++){
        inicio = gerar_numero_aletorio(tam);
        if(tipoGrafo == 1){
            bfs = BFS_lista(grafoLista,inicio);
        }else{
            bfs = BFS_matriz(grafoMat,inicio);
        }
        if(!bfs.valido) return -2.0; //erro de execucao
        
        liberar_bfs(bfs);
        soma_tempo += bfs.tempo_execucao;
    }
    double resultado = (double) soma_tempo / 100.0; //media de tempo em milisegundos
    return resultado;
}
double DFS_100(Grafo_Matriz *gMatriz, Grafo_lista *gLista,int tipoGrafo){
    clock_t inicio,fim;
    int verticeInicial;
    int tam = (tipoGrafo == 1) ? gLista->tam : gMatriz->tam;

    double tempo_execucao = 0;
    for(int cont = 0; cont < 100 ; cont++){
        int *pai = malloc(sizeof(int) * tam);
        int *nivel = malloc(sizeof(int) * tam);
        
        verticeInicial = gerar_numero_aletorio(tam);
        inicio = clock();
        if(tipoGrafo == 1){
            DFSLista(gLista,verticeInicial,pai,nivel);
        }else{
            DFSMatriz(gMatriz,verticeInicial,pai,nivel);
        }
        fim = clock();
        
        free(pai); free(nivel);
        tempo_execucao += calcular_execucao(inicio,fim);
    }
    double resultado = (double) tempo_execucao / 100.0; //media de tempo em milisegundos
    return resultado;
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
    origem = clock();
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
    liberar_fila(fila);
    
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
    liberar_fila(fila);

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

    int distancia = bfs.nivel[index_pai];
    liberar_bfs(bfs);
    return distancia;
}
int distancia_pares_matriz(Grafo_Matriz *g,int inicio,int index_pai){
    ResultadoBFS bfs = BFS_matriz(g,inicio);

    int distancia = bfs.nivel[index_pai];
    liberar_bfs(bfs);
    return distancia;
}
int pai_vertices(int *pai,int vertice){
    return pai[vertice];
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
    if(tam > 50000) return NULL; // limite de ram, acima disso apenas 16gb pra cima
    
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
void ler_inserir_grafo(char nome_arq[],Grafo_Matriz *grafoMat, Grafo_lista *grafoAdj, int tipoGrafo){
    
    FILE *file = fopen(nome_arq,"r");
    if(file == NULL)return;

    int origem, destino,lixo;
    clock_t inicio,fim;

    fscanf(file,"%d",&lixo);
    int tam = (tipoGrafo == 1) ? grafoAdj->tam : grafoMat->tam; 
    while(fscanf(file,"%d %d",&origem,&destino) == 2){
        origem--; destino--; //para inserir dentro dos limites do vetor
        if(origem == -1 || destino == -1)printf("nao é possivel");
        if(origem >= tam|| destino >= tam) printf("fora do limite: %d %d tam: %d\n",origem,destino,tam);
        // 1 == listaAdjascente  // 2 == matrizAdjascente
        if(tipoGrafo == 1){  
            insercao_aresta_lista(grafoAdj, origem, destino);
        } else {
            insercao_aresta_matriz(grafoMat,origem,destino);
        }
    }   
    fclose(file);
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
Estatisticas estatisticas_lista(Grafo_lista *g){
    return calcular_estatisticas_base(g->graus, g->tam, g->numArestas);
}
Estatisticas estatisticas_matriz(Grafo_Matriz *g){
    return calcular_estatisticas_base(g->graus, g->tam, g->numArestas);
}
Estatisticas calcular_estatisticas_base(int *vetor_graus, int numVertices, int numArestas) {
    Estatisticas est;
    est.valido = true;
    int minGrau = -1, maxGrau = -1, somaGrau = 0;
   
    int *copia_graus = malloc(numVertices * sizeof(int));
    if(copia_graus == NULL){
        est.valido = false;
        return est;
    }

    for(int i = 0; i < numVertices ; i++){
        int grau = vetor_graus[i + 1]; 
        copia_graus[i] = grau;
        somaGrau += grau;
        
        if(i == 0){
            maxGrau = grau;
            minGrau = grau;
        } else {
            if(grau > maxGrau) maxGrau = grau;
            if(grau < minGrau) minGrau = grau;
        }
    }
   
    est.grauMinimo = minGrau;
    est.grauMaximo = maxGrau;
    est.grauMedio  = (double)somaGrau / (double)numVertices;
    est.grauMediana = mediana(copia_graus, numVertices);
    est.numArestas = numArestas;
    est.numVertices= numVertices;

    free(copia_graus);
    return est;
}

//arquivo estudo de casos
void escrever_arquivo(EstudoCaso e,char nomeArquivo[],int opcao){
    FILE *saida = fopen("saida_grafo.txt", "w");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saida.\n");
        return;
    }

    fprintf(saida, "================================================\n");
    fprintf(saida, "           RESULTADOS DO GRAFO\n");
    fprintf(saida, "================================================\n\n");
    fprintf(saida, "Arquivo de entrada : %s\n", nomeArquivo);
    fprintf(saida, "Representacao      : %s\n\n", opcao == 1 ? "Lista de adjacencia" : "Matriz de adjacencia");
    fprintf(saida, "================================================\n");
    fprintf(saida, "  ESTATISTICAS DO GRAFO\n");
    fprintf(saida, "================================================\n\n");
    fprintf(saida, "Numero de vertices : %d\n", e.status.numVertices);
    fprintf(saida, "Numero de arestas  : %d\n", e.status.numArestas);
    fprintf(saida, "Grau minimo        : %d\n", e.status.grauMinimo);
    fprintf(saida, "Grau maximo        : %d\n", e.status.grauMaximo);
    fprintf(saida, "Grau medio         : %.2f\n", e.status.grauMedio);
    fprintf(saida, "Mediana de grau    : %.2f\n\n", e.status.grauMediana);
    fprintf(saida, "================================================\n");
    fprintf(saida, "  COMPONENTES CONEXAS\n");
    fprintf(saida, "================================================\n\n");
    fprintf(saida, "Numero de componentes: %d\n\n", e.numComp);

    for (int i = 0; i < e.numComp; i++) {
        fprintf(saida, "Componente %d (tamanho: %d)\n", i + 1, e.comp[i].tamanho);
        fprintf(saida, "Vertices: ");
        for (int j = 0; j < e.comp[i].tamanho; j++) {
            fprintf(saida, "%d ", e.comp[i].vertices[j]);
        }
        fprintf(saida, "\n\n");
    }
    
    fclose(saida);
}

// DFS - Erick

void DFSVisitaLista(Grafo_lista *g, int v, int *visitado, int *pai, int *nivel){
    visitado[v] = 1;

    Vertice *atual = g->listaAdj[v];

    while(atual != NULL){
        int u = atual->id;

        if(!visitado[u]){
            pai[u] = v;
            nivel[u] = nivel[v] + 1;
            DFSVisitaLista(g,u,visitado,pai,nivel);
        }

        atual = atual->prox;
    }
}

void DFSLista(Grafo_lista *g, int verticeInicial, int *pai, int *nivel){

    int n = g->tam;

    int *visitado = calloc(n,sizeof(int));

    for(int i=0;i<n;i++){
        pai[i] = -1;
        nivel[i] = -1;
    }

    pai[verticeInicial] = -1;
    nivel[verticeInicial] = 0;

    DFSVisitaLista(g,verticeInicial,visitado,pai,nivel);

    free(visitado);
}


// DFS Matriz - Erick
void DFSVisitaMatriz(Grafo_Matriz *g, int v, int *visitado, int *pai, int *nivel){

    visitado[v] = 1;

    for(int u = 0; u < g->tam; u++){

        if(g->matriz[v][u] && !visitado[u]){

            pai[u] = v;
            nivel[u] = nivel[v] + 1;

            DFSVisitaMatriz(g, u, visitado, pai, nivel);
        }
    }
}

void DFSMatriz(Grafo_Matriz *g, int verticeInicial, int *pai, int *nivel){

    int n = g->tam;

    int *visitado = calloc(n, sizeof(int));

    for(int i = 0; i < n; i++){
        pai[i] = -1;
        nivel[i] = -1;
    }

    pai[verticeInicial] = -1;
    nivel[verticeInicial] = 0;

    DFSVisitaMatriz(g, verticeInicial, visitado, pai, nivel);

    free(visitado);
}

void imprimirArvoreDFS(int *pai, int *nivel, int numVertices, const char *nomeArquivo){

    FILE *arquivo = fopen(nomeArquivo, "w");

    if(arquivo == NULL){
        printf("Erro ao abrir arquivo de saida.\n");
        return;
    }

    fprintf(arquivo, "Vertice Pai Nivel\n");

    for(int i = 0; i < numVertices; i++){
        fprintf(arquivo,"%d %d %d\n", i, pai[i], nivel[i]);
    }

    fclose(arquivo);
}


// Componentes Conexas - Erick

int compararComponentes(const void *a, const void *b){
    Componente *compA = (Componente *)a;
    Componente *compB = (Componente *)b;

    return compB->tamanho - compA->tamanho;
}

void DFS_ComponenteLista(Grafo_lista *g, int v, int *visitado,int *listaVertices, int *tamanho){

    visitado[v] = 1;
    listaVertices[(*tamanho)] = v;
    (*tamanho)++;

    Vertice *atual = g->listaAdj[v];

    while(atual != NULL){

        int u = atual->id;

        if(!visitado[u]){
            DFS_ComponenteLista(g, u, visitado, listaVertices, tamanho);
        }

        atual = atual->prox;
    }
}

Componente *componentesConexasLista(Grafo_lista *g, int *numComponentes){

    int n = g->tam;

    int *visitado = calloc(n, sizeof(int));

    Componente *componentes = malloc(n * sizeof(Componente));

    int contador = 0;

    for(int i = 0; i < n; i++){
        if(!visitado[i]){
            int *listaTemp = malloc(n * sizeof(int));
            int tamanho = 0;
            /// DFS sem recursao 
            DFS_ComponenteLista(g, i, visitado, listaTemp, &tamanho);

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

    if(tmp != NULL)
        componentes = tmp;

    *numComponentes = contador;

    free(visitado);

    qsort(componentes,
          contador,
          sizeof(Componente),
          compararComponentes);

    return componentes;
}

void DFS_ComponenteMatriz(Grafo_Matriz *g, int v, int *visitado,int *listaVertices, int *tamanho){

    visitado[v] = 1;
    listaVertices[(*tamanho)] = v;
    (*tamanho)++;
    for(int u = 0; u < g->tam; u++){

        if(g->matriz[v][u] && !visitado[u]){

            DFS_ComponenteMatriz(g,u,visitado, listaVertices,tamanho);
        }
    }
}

Componente *componentesConexasMatriz(Grafo_Matriz *g, int *numComponentes){
    int n = g->tam;
    int *visitado = calloc(n, sizeof(int));
    Componente *componentes = malloc(n * sizeof(Componente));

    int contador = 0;

    for(int i = 0; i < n; i++){

        if(!visitado[i]){

            int *listaTemp = malloc(n * sizeof(int));

            int tamanho = 0;

            DFS_ComponenteMatriz(g,i,visitado, listaTemp, &tamanho);
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

    if(tmp != NULL)
        componentes = tmp;

    *numComponentes = contador;

    free(visitado);

    qsort(componentes,contador,sizeof(Componente), compararComponentes);

    return componentes;
}

void escreverComponentes(const char *nomeArquivo,Componente *componentes,int numComponentes){

    FILE *arquivo = fopen(nomeArquivo, "w");

    if(arquivo == NULL){
        printf("Erro ao abrir arquivo de saida.\n");
        return;
    }

    fprintf(arquivo,"Numero de componentes: %d\n\n",numComponentes);

    for(int i = 0; i < numComponentes; i++){

        fprintf(arquivo,"Componente %d\n",i + 1);

        fprintf(arquivo,"Tamanho: %d\n",componentes[i].tamanho);

        fprintf(arquivo,"Vertices: ");

        for(int j = 0;j < componentes[i].tamanho;j++){

            fprintf(arquivo, "%d ",componentes[i].vertices[j]);
        }
        fprintf(arquivo,"\n\n");
    }

    fclose(arquivo);
}

int distanciaLista(Grafo_lista *g, int origem, int destino){
    ResultadoBFS bfs = BFS_lista(g, origem);

    int distancia = bfs.nivel[destino];

    liberar_bfs(bfs);

    return distancia;
}

int distanciaMatriz(Grafo_Matriz *g, int origem, int destino){
    ResultadoBFS bfs = BFS_matriz(g, origem);

    int distancia = bfs.nivel[destino];

    liberar_bfs(bfs);

    return distancia;
}

int diametroLista(Grafo_lista *g){

    int diametro = 0;

    for(int i = 0; i < g->tam; i++){

        ResultadoBFS bfs = BFS_lista(g, i);

        if(!bfs.valido)
            return -1;

        for(int j = 0; j < g->tam; j++){

            if(bfs.nivel[j] > diametro){
                diametro = bfs.nivel[j];
            }
        }

        liberar_bfs(bfs);
    }

    return diametro;
}

int diametroMatriz(Grafo_Matriz *g){

    int diametro = 0;

    for(int i = 0; i < g->tam; i++){

        ResultadoBFS bfs = BFS_matriz(g, i);

        if(!bfs.valido)
            return -1;

        for(int j = 0; j < g->tam; j++){

            if(bfs.nivel[j] > diametro){
                diametro = bfs.nivel[j];
            }
        }

        liberar_bfs(bfs);
    }

    return diametro;
}