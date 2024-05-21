#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "protocolo_top.h"


/*
   Função para analisar a resposta recebida.
   
   Parâmetros:
   - response: a resposta recebida após a solicitação de junção
   - mode: uma variável de saída para indicar o modo de operação (vazio ou com nós)
   - node_to_join: uma variável de saída para armazenar informações sobre o nó a se juntar
   - id: o ID do nó atual
   
   Funcionamento:
   1. Cria uma cópia da resposta para manipulação.
   2. Percorre a resposta token por token, contando o número de nós presentes e verificando se o ID do nó atual já está na lista.
   3. Se o ID do nó atual já estiver na lista, gera um novo ID aleatório até encontrar um que não esteja na lista.
   4. Define o modo de operação com base no número de nós presentes na resposta.
   5. Seleciona aleatoriamente um nó na lista (exceto o próprio nó) para se juntar.
*/
void analize_response_for_join(char* response, char* mode, char* node_to_join, char *id);


/*
   Esta função analisa a resposta recebida após uma solicitação de criação de corda (chord).
   
   Parâmetros:
   - Nodei: o nó atual
   - response: a resposta recebida após a solicitação de criação de corda
   - mode: uma variável de saída para indicar se há nós disponíveis para criação de corda
   - node_to_join: uma variável de saída para armazenar informações sobre o nó a se juntar
   - chords: um array que mantém o estado das conexões de corda com outros nós
   
   Funcionamento:
   1. Cria uma cópia da resposta para manipulação.
   2. Percorre a resposta token por token, contando o número de nós presentes e verificando se o nó atual, seu sucessor, seu predecessor ou qualquer nó com o qual já existe uma corda estão na lista.
   3. Calcula o número de nós disponíveis para criação de corda.
   4. Se houver nós disponíveis, seleciona aleatoriamente um nó na lista que não seja o nó atual, seu sucessor, seu predecessor ou qualquer nó com o qual já existe uma corda.
   5. Define o modo de operação como "No_Chord" se não houver nós disponíveis para criação de corda.
*/
void analize_response_for_chord(struct Node *node, char* response, char* mode, char* node_to_join, int chords[]);

#endif