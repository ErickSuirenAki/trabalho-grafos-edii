#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grafo.h"
#include <stdbool.h>

void opcoes_menu(){
    puts("\n--- MENU GRAFOS ---");
    puts("1 - Gerar arquivo estudo");
    puts("2 - Realizar 100 DFS e BFS");
    puts("3 - Determine o pai de um vertice(DFS e BFS)");
    puts("4 - Distancia entre dois pares");
    puts("5 - Diametro de um grafo");
    puts("6 - Fechar programa");
}

void gerar_estudo(int tipoGrafo, Grafo_lista *gLista, Grafo_Matriz *gMatriz, char nomeArquivo[]) {
    EstudoCaso estudo;
    int numComp;

    if (tipoGrafo == 1) {
        estudo.status = estatisticas_lista(gLista);
        estudo.comp = componentesConexasLista(gLista, &numComp);
    } else {
        estudo.status = estatisticas_matriz(gMatriz);
        estudo.comp = componentesConexasMatriz(gMatriz, &numComp);
    }
    
    estudo.numComp = numComp;
    escrever_arquivo(estudo, nomeArquivo, tipoGrafo);
    char *nomeTipoGrafo = (tipoGrafo == 1) ? "Lista Adjascente" : "Matriz Adjascente";
    printf("\nArquivo de caso do %s (saida_grafo.txt) gerado: %s\n", nomeTipoGrafo,nomeArquivo);
}

bool inicioValido(int tam, int inicio){
    if(inicio < 0 || inicio > tam -1){
        puts("Inicio invalido,tente novamente");
        return false;
    } 
    return true;
}
void menu_grafo(int tipoGrafo, Grafo_lista *gLista, Grafo_Matriz *gMatriz, char nomeArquivo[]) {
    while(1) {
        opcoes_menu();
        printf("\nQual opcao deseja: ");
        int opt;
        if (scanf("%d", &opt) != 1) break; 
        
        switch(opt){
            case 1: { //1 - Gerar arquivo estudo 
                gerar_estudo(tipoGrafo, gLista, gMatriz, nomeArquivo);
                break;
            }
            case 2: { //2 - Realizar 100 DFS e BFS 
                double tempoBFS, tempoDFS;
                
                tempoBFS = BFS_100(gMatriz,gLista,tipoGrafo);
                tempoDFS = DFS_100(gMatriz,gLista,tipoGrafo);

                printf("\nTempo medio execucao BFS: %.2f ms", tempoBFS);
                printf("\nTempo medio execucao DFS: %.2f ms\n", tempoDFS);
                break;
            } 
            case 3: { //3 - Determine o pai de um vertice(DFS e BFS) 
                int tam = (tipoGrafo == 1) ? gLista->tam : gMatriz->tam;
                int inicio =0, vertice;
                printf("Insira o vertice de inicio do BFS/DFS(0 e %d): ",tam-1);
                scanf("%d", &inicio);
                if(!inicioValido(tam,inicio)){
                    return;
                }
                
                int *dfsPai = malloc(sizeof(int) * tam);
                int *dfsNivel = malloc(sizeof(int) * tam);
                
                ResultadoBFS bfs;

                if (tipoGrafo == 1) {
                    bfs = BFS_lista(gLista, inicio);
                    DFSLista(gLista, inicio, dfsPai, dfsNivel);
                } else {
                    bfs = BFS_matriz(gMatriz, inicio);
                    DFSMatriz(gMatriz, inicio, dfsPai, dfsNivel);
                }
                
                printf("Insira o vertice desejado para buscar o pai(0 e %d): ",tam-1);
                scanf("%d", &vertice);
                if(!inicioValido(tam,vertice)){
                    return;
                }
                
                int paiBFS = pai_vertices(bfs.pai, vertice);
                int paiDFS = pai_vertices(dfsPai, vertice);
                
                printf("\nPai BFS do vertice %d (inicio %d): %d", vertice, inicio, paiBFS);
                printf("\nPai DFS do vertice %d (inicio %d): %d\n", vertice, inicio, paiDFS);
                
                liberar_bfs(bfs);
                free(dfsPai);
                free(dfsNivel);
                break;
            }
            case 4: { //4 - Distancia entre dois pares 
                int inicio, idxVertice;
                printf("Insira os dois vertices para calcular a distancia (origem destino): ");
                scanf("%d %d", &inicio, &idxVertice);
                
                int tam = (tipoGrafo == 1) ? gLista->tam : gMatriz->tam;
                int *dfsPai = malloc(sizeof(int) * tam);
                int *dfsNivel = malloc(sizeof(int) * tam);
                
                int resultadoBfs;
                
                if (tipoGrafo == 1) {
                    resultadoBfs = distancia_pares_lista(gLista, inicio, idxVertice);
                    DFSLista(gLista, inicio, dfsPai, dfsNivel);
                } else {
                    resultadoBfs = distancia_pares_matriz(gMatriz, inicio, idxVertice);
                    DFSMatriz(gMatriz, inicio, dfsPai, dfsNivel);
                }

                int resultadoDfs = dfsNivel[idxVertice];

                printf("\nDistancia BFS de (%d,%d): %d", inicio, idxVertice, resultadoBfs);
                printf("\nDistancia DFS de (%d,%d): %d\n", inicio, idxVertice, resultadoDfs);
                
                free(dfsPai);
                free(dfsNivel);
                break;
            }
            case 5: { //5 - Diametro de um grafo
                // a complexidade do algoritmo é o(n^3), precisa implementar um aproximativo para grafos grandes
                int diametro = (tipoGrafo == 1) ? diametroLista(gLista) : diametroMatriz(gMatriz);
                printf("\nDiametro do grafo: %d\n", diametro);
                break;
            }
            case 6:   //6 - Fechar programa
                return;
            default:
                puts("\nOpcao invalida. Tente novamente.");
        }
    }
}

int main() {
    char nomeArquivo[100];
    srand(time(NULL)); //iniciailizar numero aleatorio
    printf("Digite o nome do arquivo do grafo: ");
    scanf("%s", nomeArquivo);

    int n = numero_vetores(nomeArquivo);
    int m = numero_arestas(nomeArquivo);

    if (n <= 0) {
        printf("Erro ao ler o grafo. Numero de vertices invalido.\n");
        return 1;
    }

    int tipoGrafo = 0;
    while (tipoGrafo != 1 && tipoGrafo != 2) {
        printf("\nEscolha a representacao do grafo:\n");
        printf("1 - Lista de adjacencia\n");
        printf("2 - Matriz de adjacencia\n");
        printf("Opcao: ");
        scanf("%d", &tipoGrafo);
        if(tipoGrafo != 1 && tipoGrafo != 2) puts("Opcao invalida.");
    }

    Grafo_lista  *gLista  = NULL; 
    Grafo_Matriz *gMatriz = NULL;

    if (tipoGrafo == 1) {
        gLista = iniciarGrafoLista(n, m);
        if(gLista == NULL){
            puts("ERRO: sem memoria para Lista");
            return 1;
        }
    } else {
        gMatriz = iniciarGrafoMatriz(n);
        if(gMatriz == NULL){
            puts("ERRO: sem memoria para matriz");
            return 1;
        }
    }

    ler_inserir_grafo(nomeArquivo,gMatriz,gLista,tipoGrafo);   

    menu_grafo(tipoGrafo, gLista, gMatriz, nomeArquivo);

    liberar_lista(gLista);
    liberar_matriz(gMatriz, n);
   
    return 0;
}