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
        puts("gerando estatisticas...");
        estudo.status = estatisticas_lista(gLista);
        puts("gerando componentesLista");
        estudo.comp = componentesConexasLista(gLista, &numComp);
    } else {
        puts("gerando estatisticas...");
        estudo.status = estatisticas_matriz(gMatriz);
        puts("gerando componentesLista");
        estudo.comp = componentesConexasMatriz(gMatriz, &numComp);
    }
    
    estudo.numComp = numComp;
    puts("escrevendo o arquivo...");
    escrever_arquivo(estudo, nomeArquivo, tipoGrafo);
    char *nomeTipoGrafo = (tipoGrafo == 1) ? "Lista Adjacente" : "Matriz Adjacente";
    printf("\nArquivo de caso do %s (saida_grafo.txt) gerado: %s\n", nomeTipoGrafo,nomeArquivo);
    
    free(estudo.comp->vertices);
    free(estudo.comp);
}

bool inicioValido(int tam, int inicio){
    if(inicio < 0 || inicio > tam){
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
                printf("Insira o vertice de inicio do BFS/DFS(1 e %d): ",tam);
                scanf("%d", &inicio);
                inicio--; //valor dentro do vetor
                if(!inicioValido(tam,inicio)){
                    return;
                }
                
                int *dfsVisitado = malloc(sizeof(int) * tam);
                int *dfslistVertice = malloc(sizeof(int) * tam);
                int tamdfs = 0;
                
                ResultadoBFS bfs;

                if (tipoGrafo == 1) {
                    bfs = BFS_lista(gLista, inicio);
                    DFSLista(gLista, inicio, dfsVisitado, dfslistVertice,&tamdfs);
                } else {
                    bfs = BFS_matriz(gMatriz, inicio);
                    DFSMatriz(gMatriz, inicio, dfsVisitado, dfslistVertice,&tamdfs);
                }
                printf("\nquantos pais deseja buscar?: ");
                int rep;
                scanf("%d",&rep);

                while(rep > 0){
                    printf("Insira o vertice desejado para buscar o pai(1 e %d): ",tam);
                    scanf("%d", &vertice);
                    vertice--; //valor dentro do vetor
                    if(!inicioValido(tam,vertice)){
                        return;
                    }
                    
                    int paiBFS = pai_vertices(bfs.pai, vertice);
                    int paiDFS = pai_vertices(dfslistVertice, vertice);
                    
                    printf("\nPai BFS do vertice %d (inicio %d): %d", vertice+1, inicio+1, paiBFS);
                    printf("\nPai DFS do vertice %d (inicio %d): %d\n", vertice+1, inicio+1, paiDFS);
                    rep--;
                }
                
               
                
                liberar_bfs(bfs);
                free(dfsVisitado);
                free(dfslistVertice);
                break;
            }
            case 4: { //4 - Distancia entre dois pares 
                int inicio, idxVertice;
                int tam = (tipoGrafo == 1) ? gLista->tam : gMatriz->tam;
                printf("Insira os dois vertices para calcular a distancia (1 %d): ",tam);
                scanf("%d %d", &inicio, &idxVertice);
                inicio--; idxVertice--; //valor dentro do vetor
                
                int *dfsVisitado = malloc(sizeof(int) * tam);
                int *dfsListVertice = malloc(sizeof(int) * tam);
                int dfstam = 0;

                int paiBfs;
                
                if (tipoGrafo == 1) {
                    paiBfs = distancia_pares_lista(gLista, inicio, idxVertice);
                    DFSLista(gLista, inicio, dfsVisitado, dfsListVertice,&dfstam);
                } else {
                    paiBfs = distancia_pares_matriz(gMatriz, inicio, idxVertice);
                    DFSMatriz(gMatriz, inicio, dfsVisitado, dfsListVertice,&dfstam);
                }

                int paiDfs = dfsListVertice[idxVertice];

                printf("\nDistancia BFS de (%d,%d): %d", inicio+1, idxVertice+1, paiBfs);
                printf("\nDistancia DFS de (%d,%d): %d\n", inicio+1, idxVertice+1, paiDfs);
                
                free(dfsVisitado);
                free(dfsListVertice);
                break;
            }
            case 5: { //5 - Diametro de um grafo
                // a complexidade do algoritmo é o(n^3), precisa implementar um aproximativo para grafos grandes
                int u, v;
                int diametro = diametroAproximado(gLista,gMatriz,&u,&v);
                printf("\nDiametro do grafo entre o vertice(%d,%d): %d\n",u,v, diametro);
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
    printf("Lendo arquivo: %s...\n",nomeArquivo);
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
    char *tipog = (tipoGrafo == 1) ? "Lista adjacente" : "Matriz adjacente";
    printf("\nIniciando o %s..",tipog);
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
    puts("\nInserindo as arestas...");
    ler_inserir_grafo(nomeArquivo,gMatriz,gLista,tipoGrafo);   

    menu_grafo(tipoGrafo, gLista, gMatriz, nomeArquivo);

    liberar_lista(gLista);
    liberar_matriz(gMatriz, n);
   
    return 0;
}