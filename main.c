#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

void teste(Estatisticas est){
    printf("Grau Minimo: %d\n",est.grauMinimo);
    printf("Grau Medio: %f\n",est.grauMedio);
    printf("Grau Mediana: %f\n",est.grauMediana);
    printf("Grau Maximo: %d\n",est.grauMaximo);
    printf("Numero de vertices: %d\n",est.numVertices);
    printf("Numero de arestas: %d\n",est.numArestas);
}
void ler_bfs(ResultadoBFS bfs, int inicio){
    printf("Caminho = ");
    for(int i = 0; i < bfs.tamanho; i++){
        printf("%d ", bfs.caminho[i]);
    }
    printf("\n");

    printf("Pai = ");
    for(int i = 1; i <= bfs.tamanho; i++){
        printf("%d ", bfs.pai[i]);
    }
    printf("\n");
    printf("Nivel = ");
    for(int i = 1; i <= bfs.tamanho; i++){
        printf("%d ", bfs.nivel[i]);
    }

}

void calcular_memoria_lista(long V, long E, int direcionado) {

    long long arestas_armazenadas = direcionado ? (long long)E : 2LL * E;

    long long bytes = 
        sizeof(Grafo_lista) +                           
        (long long)V * sizeof(Vertice*) +              
        (long long)V * sizeof(int) +                   
        arestas_armazenadas * sizeof(Vertice);          

    double mb = (double)bytes / 1048576.0;
    double gb = mb / 1024.0;
    if (gb >= 1.0) {
        printf("Memoria estimada da Lista: %.2f GB\n", gb);
    } else {
        printf("Memoria estimada da Lista: %.2f MB\n", mb);
    }
}
void calcular_memoria_matriz(int tam){
    long long bytes = 
        sizeof(Grafo_Matriz) +                          
        (long long)tam * sizeof(bool*) +                 
        (long long)tam * sizeof(int) +                 
        (long long)tam * tam * sizeof(bool);            

    double mb = (double)bytes / 1048576.0;
    
    double gb = mb / 1024.0;
    if (gb >= 1.0) {
        printf("Memoria estimada da Matriz: %.2f GB\n", gb);
    } else {
        printf("Memoria estimada da Matriz: %.2f MB\n", mb);
    }
}


double execuxao_lista(char nome_arquivo[],Grafo_lista *grafoAdj){
    double tempo_execucao = 0;
    
    clock_t inicio,fim;
    puts("\n==== Lista Adjacente ====");
    
    puts("Lendo o arquivo e inserindo as arestas...");
    tempo_execucao += ler_inserir_lista(nome_arquivo,grafoAdj);
    
    puts("Grafo lista carregado com sucesso!!");
    puts("Calculando as estatisticas do grafo...");
    
    inicio = clock();
    Estatisticas estLista = estatisticas_lista(grafoAdj);
    fim = clock();
    tempo_execucao += calcular_execucao(inicio,fim);
    
    puts("Estastiticas lista carregado com sucesso!");
    puts("Realizando BFS...");
    
    inicio = clock();
    ResultadoBFS bfs = BFS_lista(grafoAdj,1);
    fim = clock();
    tempo_execucao += calcular_execucao(inicio,fim);
    
    puts("BFS carregado com sucesso!!");
    puts("Imprimindo BFS: ");
    ler_bfs(bfs,1);
    puts("\nImprimindo estastiticas Listas:");
    teste(estLista);
    
    return tempo_execucao;
}

double execuxao_matriz(char nome_arquivo[],Grafo_Matriz *grafoMat){
    double tempo_execucao = 0;
    clock_t inicio,fim;

    puts("\n==== Matriz Adjascente ====");
    
    puts("Lendo o arquivo e inserindo as arestas...");
    tempo_execucao += ler_inserir_matriz(nome_arquivo,grafoMat);
    puts("Grafo Matriz carregado com sucesso!!");
    
    puts("Calculando as estatisticas do grafo...");
    inicio = clock();
    Estatisticas estMatriz = estatisticas_matriz(grafoMat);
    fim = clock();
    tempo_execucao += calcular_execucao(inicio,fim);
    puts("Estastiticas matriz carregado com sucesso!");


    puts("Realizando BFS...");
    inicio = clock();
    ResultadoBFS bfs = BFS_matriz(grafoMat,1);
    fim = clock();
    tempo_execucao += calcular_execucao(inicio,fim);
    
    puts("BFS carregado com sucesso!!");
    puts("Imprimindo BFS: ");
    ler_bfs(bfs,1);
    puts("\nImprimindo estastiticas Matrizes:");
    teste(estMatriz);

    return tempo_execucao;
}

int main(int argc, char const *argv[])
{
   
    clock_t inicio, fim;
        char nome_arquivo[] = "testeBFS.txt";

        double tempo_execucao_lista = 0;
        double tempo_execucao_matriz = 0;

        printf("\n==== %s ====\n",nome_arquivo);
        
        if(!abrirArquivo(nome_arquivo)){
            printf("ERRO: Nao foi possivel abrir o arquivo: %s",nome_arquivo);
            return 0;
        }

        puts("Lendo o arquivo...");
        int tam = numero_vetores(nome_arquivo) + 1;
        int numArestas = numero_arestas(nome_arquivo);


        puts("Carregando a Lista na memoria...");
        inicio  = clock();
        Grafo_lista *grafoAdj = iniciarGrafoLista(tam,numArestas);
        if (grafoAdj == NULL) {
            puts("ERRO: memoria insuficiciente para lista adjacente");
            printf("Tamanho: ");
            calcular_memoria_lista(tam-1,numArestas,2);
        }
        fim = clock();
        tempo_execucao_lista += calcular_execucao(inicio,fim);

        puts("Carregando a Matriz na memoria...");
        inicio  = clock();
        Grafo_Matriz *grafoMat = iniciarGrafoMatriz(tam);
        if (grafoMat == NULL) {
            puts("ERRO: memoria insuficiciente para matriz adjacente");
            printf("Tamanho: ");
            calcular_memoria_matriz(tam);
        }
        fim = clock();
        tempo_execucao_matriz += calcular_execucao(inicio,fim);

        if(grafoAdj != NULL){
            tempo_execucao_lista += execuxao_lista(nome_arquivo,grafoAdj);
            printf("\nTempo de execucao Lista: %.0fms\n",tempo_execucao_lista);
            calcular_memoria_lista(tam-1,numArestas,2);
        }
        if(grafoMat != NULL){
            tempo_execucao_matriz += execuxao_matriz(nome_arquivo,grafoMat);
            printf("\nTempo de execucao Matriz: %.0fms\n",tempo_execucao_matriz);

            calcular_memoria_matriz(tam);
        }


        liberar_lista(grafoAdj);
        puts("\nGrafo lista liberado com sucesso!");
        liberar_matriz(grafoMat,tam);
        puts("\nGrafo matriz liberado com sucesso!");

    
    
    return 0;
}