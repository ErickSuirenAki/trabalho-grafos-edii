#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grafo.h"
#include <stdbool.h>

void opcoes_menu(){
    puts("\n--- MENU GRAFOS ---");
    puts("1 - Gerar arquivo estudo");
    puts("2 - Realizar 100 DFS e BFS");
    puts("3 - Determine o pai de um vertice");
    puts("4 - Distancia entre dois pares");
    puts("5 - Diametro de um grafo");
    puts("6 - Estudo de caso completo (itens 1 ao 7)");
    puts("7 - Fechar programa");
}

void gerar_estudo(int tipoGrafo, Grafo_lista *gLista, Grafo_Matriz *gMatriz, char nomeArquivo[]) {
    EstudoCaso estudo;
    int numComp;

    if (tipoGrafo == 1) {
        puts("gerando estatisticas...");
        estudo.status = estatisticas_lista(gLista);
        puts("gerando componentes...");
        estudo.comp = componentesConexasLista(gLista, &numComp);
    } else {
        puts("gerando estatisticas...");
        estudo.status = estatisticas_matriz(gMatriz);
        puts("gerando componentes...");
        estudo.comp = componentesConexasMatriz(gMatriz, &numComp);
    }

    estudo.numComp = numComp;
    puts("escrevendo o arquivo...");
    escrever_arquivo(estudo, nomeArquivo, tipoGrafo);

    char *nomeTipoGrafo = (tipoGrafo == 1) ? "Lista Adjacente" : "Matriz Adjacente";
    printf("\nArquivo de caso do %s (saida_grafo.txt) gerado: %s\n", nomeTipoGrafo, nomeArquivo);

    for (int i = 0; i < numComp; i++) free(estudo.comp[i].vertices);
    free(estudo.comp);
}

bool inicioValido(int tam, int inicio){
    if (inicio < 0 || inicio >= tam){
        puts("Inicio invalido, tente novamente");
        return false;
    }
    return true;
}

void menu_grafo(int tipoGrafo, Grafo_lista *gLista, Grafo_Matriz *gMatriz, char nomeArquivo[]) {
    while (1) {
        opcoes_menu();
        printf("\nQual opcao deseja: ");
        int opt;
        if (scanf("%d", &opt) != 1) break;

        switch (opt) {

            case 1: { // Gerar arquivo estudo
                gerar_estudo(tipoGrafo, gLista, gMatriz, nomeArquivo);
                break;
            }

            case 2: { // Realizar 100 DFS e BFS
                double tempoBFS = BFS_100(gMatriz, gLista, tipoGrafo);
                double tempoDFS = DFS_100(gMatriz, gLista, tipoGrafo);
                printf("\nTempo medio execucao BFS: %.2f ms", tempoBFS);
                printf("\nTempo medio execucao DFS: %.2f ms\n", tempoDFS);
                break;
            }

            case 3: { // Pai de um vertice via BFS
                int tam = (tipoGrafo == 1) ? gLista->tam : gMatriz->tam;
                int inicio = 0, vertice;

                printf("Insira o vertice de inicio da BFS (1 a %d): ", tam);
                scanf("%d", &inicio);
                inicio--;

                if (!inicioValido(tam, inicio)) break;

                ResultadoBFS bfs = (tipoGrafo == 1)
                    ? BFS_lista(gLista, inicio)
                    : BFS_matriz(gMatriz, inicio);

                printf("Quantos pais deseja buscar?: ");
                int rep;
                scanf("%d", &rep);

                while (rep > 0) {
                    printf("Insira o vertice desejado (1 a %d): ", tam);
                    scanf("%d", &vertice);
                    vertice--;

                    if (!inicioValido(tam, vertice)) break;

                    int paiBFS = pai_vertices(bfs.pai, vertice);
                    printf("\nPai BFS do vertice %d (inicio %d): %d\n",
                           vertice + 1,
                           inicio + 1,
                           paiBFS == -1 ? -1 : paiBFS + 1);
                    rep--;
                }

                liberar_bfs(bfs);
                break;
            }

            case 4: { // Distancia entre dois vertices
                int tam = (tipoGrafo == 1) ? gLista->tam : gMatriz->tam;
                int inicio, idxVertice;

                printf("Insira os dois vertices (1 a %d): ", tam);
                scanf("%d %d", &inicio, &idxVertice);
                inicio--; idxVertice--;

                if (!inicioValido(tam, inicio) || !inicioValido(tam, idxVertice)) break;

                int dist = (tipoGrafo == 1)
                    ? distancia_pares_lista(gLista, inicio, idxVertice)
                    : distancia_pares_matriz(gMatriz, inicio, idxVertice);

                printf("\nDistancia entre (%d,%d): %d\n",
                       inicio + 1, idxVertice + 1, dist);
                break;
            }

            case 5: { // Diametro aproximado
                int u, v;
                int diam = diametroAproximado(
                    tipoGrafo == 1 ? gLista  : NULL,
                    tipoGrafo == 2 ? gMatriz : NULL,
                    &u, &v);
                printf("\nDiametro aproximado: %d (entre vertices %d e %d)\n",
                       diam, u + 1, v + 1);
                break;
            }

            case 6: { // Estudo de caso completo itens 1 ao 7
                int tam = (tipoGrafo == 1) ? gLista->tam : gMatriz->tam;
                int numArestas = (tipoGrafo == 1) ? gLista->numArestas : gMatriz->numArestas;

                printf("\n================================================");
                printf("\n  ESTUDO DE CASO - %s", nomeArquivo);
                printf("\n================================================\n");

                // Item 1: Memoria
                double memLista  = calcular_memoria_lista(tam, numArestas);
                double memMatriz = calcular_memoria_matriz(tam);
                printf("\n[1] MEMORIA UTILIZADA\n");
                printf("    Lista de adjacencia : %.4f MB\n", memLista);
                printf("    Matriz de adjacencia: %.4f MB\n", memMatriz);

                // Item 2: 100 BFS
                printf("\n[2] TEMPO MEDIO DE 100 BFS\n");
                double mediaBFS = BFS_100(gMatriz, gLista, tipoGrafo);
                printf("    Tempo medio: %.2f ms\n", mediaBFS);

                // Item 3: 100 DFS
                printf("\n[3] TEMPO MEDIO DE 100 DFS\n");
                double mediaDFS = DFS_100(gMatriz, gLista, tipoGrafo);
                printf("    Tempo medio: %.2f ms\n", mediaDFS);

                // Item 4: Pai dos vertices 10, 20, 30 partindo de 1, 2, 3
                printf("\n[4] PAI DOS VERTICES 10, 20, 30 (BFS e DFS)\n");
                printf("    %-10s %-12s %-12s %-12s    %-12s %-12s %-12s\n",
                    "Inicio", "BFS(10)", "BFS(20)", "BFS(30)",
                                "DFS(10)", "DFS(20)", "DFS(30)");

                int inicios[] = {0, 1, 2};
                int buscas[]  = {9, 19, 29};

                for (int i = 0; i < 3; i++) {
                    ResultadoBFS bfs = (tipoGrafo == 1)
                        ? BFS_lista(gLista, inicios[i])
                        : BFS_matriz(gMatriz, inicios[i]);

                    int *dfsPai   = malloc(tam * sizeof(int));
                    int *dfsNivel = malloc(tam * sizeof(int));

                    if (tipoGrafo == 1)
                        DFS_PaiNivel(gLista, inicios[i], dfsPai, dfsNivel);

                    printf("    %-10d %-12d %-12d %-12d    %-12d %-12d %-12d\n",
                        inicios[i] + 1,
                        bfs.pai[buscas[0]] == -1 ? -1 : bfs.pai[buscas[0]] + 1,
                        bfs.pai[buscas[1]] == -1 ? -1 : bfs.pai[buscas[1]] + 1,
                        bfs.pai[buscas[2]] == -1 ? -1 : bfs.pai[buscas[2]] + 1,
                        dfsPai[buscas[0]]  == -1 ? -1 : dfsPai[buscas[0]]  + 1,
                        dfsPai[buscas[1]]  == -1 ? -1 : dfsPai[buscas[1]]  + 1,
                        dfsPai[buscas[2]]  == -1 ? -1 : dfsPai[buscas[2]]  + 1);

                    liberar_bfs(bfs);
                    free(dfsPai);
                    free(dfsNivel);
                }

                // Item 5: Distancia entre pares
                printf("\n[5] DISTANCIA ENTRE PARES DE VERTICES\n");
                int pares[3][2] = {{9,19}, {9,29}, {19,29}};
                char *labels[3] = {"(10,20)", "(10,30)", "(20,30)"};

                for (int i = 0; i < 3; i++) {
                    int dist = (tipoGrafo == 1)
                        ? distancia_pares_lista(gLista,  pares[i][0], pares[i][1])
                        : distancia_pares_matriz(gMatriz, pares[i][0], pares[i][1]);
                    printf("    Distancia %s: %d\n", labels[i], dist);
                }
                if (tipoGrafo == 1) {
                    int *dpai   = malloc(tam * sizeof(int));
                    int *dnivel = malloc(tam * sizeof(int));

                    DFS_PaiNivel(gLista, 9, dpai, dnivel);
                    printf("\n[5b] DISTANCIA DFS partindo do vertice 10\n");
                    printf("    DFS(10,20): %d\n", dnivel[19]);
                    printf("    DFS(10,30): %d\n", dnivel[29]);

                    DFS_PaiNivel(gLista, 19, dpai, dnivel);
                    printf("    DFS(20,30): %d\n", dnivel[29]);

                    free(dpai);
                    free(dnivel);
                }

                // Item 6: Componentes conexas
                printf("\n[6] COMPONENTES CONEXAS\n");
                int numComp;
                Componente *comp = (tipoGrafo == 1)
                    ? componentesConexasLista(gLista,  &numComp)
                    : componentesConexasMatriz(gMatriz, &numComp);

                printf("    Numero de componentes: %d\n", numComp);
                printf("    Maior componente     : %d vertices\n", comp[0].tamanho);
                printf("    Menor componente     : %d vertices\n", comp[numComp-1].tamanho);

                for (int i = 0; i < numComp; i++) free(comp[i].vertices);
                free(comp);

                // Item 7: Diametro aproximado
                printf("\n[7] DIAMETRO DO GRAFO (aproximado)\n");
                int u, v;
                int diam = diametroAproximado(
                    tipoGrafo == 1 ? gLista  : NULL,
                    tipoGrafo == 2 ? gMatriz : NULL,
                    &u, &v);
                printf("    Diametro: %d (entre vertices %d e %d)\n",
                       diam, u + 1, v + 1);

                printf("\n================================================\n");
                break;
            }

            case 7: // Fechar programa
                return;

            default:
                puts("\nOpcao invalida. Tente novamente.");
        }
    }
}

int main() {
    char nomeArquivo[100];
    srand(time(NULL));

    printf("Digite o nome do arquivo do grafo: ");
    scanf("%s", nomeArquivo);
    printf("Lendo arquivo: %s...\n", nomeArquivo);

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
        if (tipoGrafo != 1 && tipoGrafo != 2) puts("Opcao invalida.");
    }

    Grafo_lista  *gLista  = NULL;
    Grafo_Matriz *gMatriz = NULL;

    char *tipog = (tipoGrafo == 1) ? "Lista adjacente" : "Matriz adjacente";
    printf("\nIniciando %s...\n", tipog);

    if (tipoGrafo == 1) {
        gLista = iniciarGrafoLista(n, m);
        if (gLista == NULL) {
            puts("ERRO: sem memoria para Lista");
            return 1;
        }
    } else {
        gMatriz = iniciarGrafoMatriz(n);
        if (gMatriz == NULL) {
            puts("ERRO: sem memoria para Matriz");
            return 1;
        }
    }

    puts("Inserindo as arestas...");
    ler_inserir_grafo(nomeArquivo, gMatriz, gLista, tipoGrafo);
    puts("Grafo carregado com sucesso!");

    menu_grafo(tipoGrafo, gLista, gMatriz, nomeArquivo);

    liberar_lista(gLista);
    liberar_matriz(gMatriz, n);

    return 0;
}
