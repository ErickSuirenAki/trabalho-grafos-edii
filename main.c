#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#define nome_arquivo "data/grafo_1.txt"
#define nome_caso "estudoCaso/estudo_grafo_1.txt"

double BFS_100_lista(Grafo_lista *grafoAdj){
    int inicio;
    ResultadoBFS bfs; 
    double soma_tempo = 0.0; 
    srand(time(NULL));
    //gera um numero aleatorio dentro do intervalo, faz o bfs e faz a media das 100 execucoes
    for(int cont = 0; cont < 100 ; cont++){
        inicio = gerar_numero_aletorio(grafoAdj->tam-1);
        bfs = BFS_lista(grafoAdj,inicio);
        
        if(!bfs.valido) return -2.0; //erro de execucao

        liberar_bfs(bfs);
        soma_tempo += bfs.tempo_execucao;
    }
    double resultado =(double) soma_tempo / 100.0; //media de tempo em milisegundos
    return resultado;
}
double BFS_100_matriz(Grafo_Matriz *grafoMat){
    int inicio;
    ResultadoBFS bfs;
    double soma_tempo = 0.0;
    srand(time(NULL));
    //gera um numero aleatorio dentro do intervalo, faz o bfs e faz a media das 100 execucoes
    for(int cont = 0; cont < 100 ; cont++){
        inicio = gerar_numero_aletorio(grafoMat->tam-1);
        bfs = BFS_matriz(grafoMat,inicio);
        
        if(!bfs.valido) return -2.0; //erro de execucao
        
        liberar_bfs(bfs);
        soma_tempo += bfs.tempo_execucao;
    }
    double resultado =(double) soma_tempo / 100.0; //media de tempo em milisegundos
    return resultado;
}

EstudoCaso execuxao_lista(Grafo_lista *grafoAdj){
    EstudoCaso estudo; estudo.valido = true;
    puts("\n==== Lista Adjacente ====");
    puts("Lendo o arquivo e inserindo as arestas...");
    
    ler_inserir_lista(nome_arquivo,grafoAdj);
    
    puts("Calculando as estatisticas do grafo...");
    
    estudo.status = estatisticas_lista(grafoAdj);
    if(!estudo.status.valido){ //erro de execucao
        estudo.valido = false;
        puts("ERRO: Memoria insuficiente durante as estatisticas");
        return estudo;
        
    }

    puts("Realizando BFS 100 vezes...");
    
    estudo.tempo_medio_BFS = BFS_100_lista(grafoAdj);
    if(estudo.tempo_medio_BFS == -1.0){ //erro de execucao
        estudo.valido = false;
        puts("ERRO: Memoria insuficiente durante BFS");
        return estudo;
    }
    
    puts("Realizando BFS pai...");
    
    bool isValido = pai_vertices_lista(grafoAdj,&estudo);
    if(!isValido){ //erro de execucao
        estudo.valido = false;
        puts("ERRO: Memoria insuficiente durante BFS");
        return estudo;
    }
    
    puts("Realizando estudo da distancia entre os pares...");
    
    estudo.distancia_pares[0] = distancia_pares_lista(grafoAdj,10,20);
    estudo.distancia_pares[1] = distancia_pares_lista(grafoAdj,10,30);
    estudo.distancia_pares[2] = distancia_pares_lista(grafoAdj,20,30);
    
    if(estudo.distancia_pares[0] == -2 || estudo.distancia_pares[1] == -2 ||  estudo.distancia_pares[2] == -2){ //erro de execucao
        estudo.valido = false;
        puts("ERRO: Memoria insuficiente durante distancia_pares");
        return estudo;
    }
    estudo.memoria_utilizada = calcular_memoria_lista(grafoAdj->tam,grafoAdj->numArestas);


    puts("Grafo carregado com sucesso!!\n");
    return estudo;
}

EstudoCaso execuxao_matriz(Grafo_Matriz *grafoMat){
    EstudoCaso estudo;
    
    puts("\n==== Matriz Adjacente ====");
    
    puts("Lendo o arquivo e inserindo as arestas...");

    ler_inserir_matriz(nome_arquivo,grafoMat);
    
    puts("Calculando as estatisticas do grafo...");
    
    estudo.status = estatisticas_matriz(grafoMat);
     if(!estudo.status.valido){ //erro de execucao
        estudo.valido = false;
        puts("ERRO: Memoria insuficiente durante as estatisticas");
        return estudo;
    }

    puts("Realizando BFS 100 vezes...");
    
    estudo.tempo_medio_BFS = BFS_100_matriz(grafoMat);
    if(estudo.tempo_medio_BFS == -2.0){ //erro de execucao
        estudo.valido = false;
        puts("ERRO: Memoria insuficiente durante BFS");
        return estudo;
    }

    puts("Realizando BFS pai...");
    
    bool isValido = pai_vertices_matriz(grafoMat,&estudo);
    if(!isValido){ //erro de execucao
        estudo.valido = false;
        puts("ERRO: Memoria insuficiente durante BFS");
        return estudo;
    }
    
    puts("Realizando estudo da distancia entre os pares...");
    
    estudo.distancia_pares[0] = distancia_pares_matriz(grafoMat,10,20);
    estudo.distancia_pares[1] = distancia_pares_matriz(grafoMat,10,30);
    estudo.distancia_pares[2] = distancia_pares_matriz(grafoMat,20,30);
    
    if(estudo.distancia_pares[0] == -2 || estudo.distancia_pares[1] == -2 ||  estudo.distancia_pares[2] == -2){ //erro de execucao
        estudo.valido = false;
        puts("ERRO: Memoria insuficiente durante distancia_pares");
        return estudo;
    }
    estudo.memoria_utilizada = calcular_memoria_matriz(grafoMat->tam);

    puts("Grafo carregado com sucesso!!\n");
    return estudo;
}

int main(int argc, char const *argv[])
{
    bool Error = false;
    printf("\n==== %s ====\n",nome_arquivo);
    
    if(!abrirArquivo(nome_arquivo)){
        printf("ERRO: Nao foi possivel abrir o arquivo: %s\n",nome_arquivo);
        puts("Encerrando programa...");
        return 0;
    }

    puts("Lendo o arquivo...");
    int tam = numero_vetores(nome_arquivo) + 1; 
    int numArestas = numero_arestas(nome_arquivo);

    puts("Carregando a Lista na memoria...");
    Grafo_lista *grafoAdj = iniciarGrafoLista(tam,numArestas);

    if (grafoAdj == NULL) { //erro de memoria
        puts("ERRO: memoria insuficiciente para lista adjacente");
        Error = true;
    }

    puts("Carregando a Matriz na memoria...");
    Grafo_Matriz *grafoMat = iniciarGrafoMatriz(tam);

    if (grafoMat == NULL) { //erro de memoria
        puts("ERRO: memoria insuficiciente para matriz adjacente");
        Error = true;
    }


    bool arquivoValido = criar_arquivo_estudo(nome_caso);
    if(!arquivoValido){ //erro na criacao do arquivo
        liberar_lista(grafoAdj);
        liberar_matriz(grafoMat,tam);
        printf("ERRO: Não foi possivel criar o arquivo caso, encerrando programa");
        return 0;
    }

    EstudoCaso estudo_lista;
    if(grafoAdj != NULL){ // faz todo estudo de caso e escreve o arquivo lista
        estudo_lista = execuxao_lista(grafoAdj);
       
        if(estudo_lista.valido){
            puts("Escrevendo arquivo do estudo de caso Lista...");
            escrever_arquivo(estudo_lista,nome_caso,"Lista Adjacente");
        }else{ //erro de execucao
            puts("ERRO: dados estudo grafo nao validos");
            Error = true;
        }

        liberar_lista(grafoAdj);
        puts("Grafo lista liberado com sucesso");
    }
    EstudoCaso estudo_matriz;
    if(grafoMat != NULL){ // faz todo estudo de caso e escreve o arquivo matriz
        estudo_matriz = execuxao_matriz(grafoMat);
        
        if(estudo_lista.valido){
            puts("Escrevendo arquivo do estudo caso Matriz");
            escrever_arquivo(estudo_matriz,nome_caso,"Matriz Adjacente");
        }else{ //erro de execucao
            puts("ERRO: dados estudo grafo nao validos");
            Error = true;
        }

        liberar_matriz(grafoMat,tam);
        puts("Grafo matriz liberado com sucesso!");
    }

    if(Error){
        printf("\nERRO: houve erros na execucao do programa\n\n");
    }else{
        printf("\nSem erros detectados durante a execucao\n\n");
    }
    
    return 0;
}