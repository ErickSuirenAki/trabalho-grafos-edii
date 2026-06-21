
#include <time.h>

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