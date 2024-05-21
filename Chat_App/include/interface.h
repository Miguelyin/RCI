#ifndef INTERFACE_H
#define INTERFACE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "protocolo_top.h"
#include "protocolo_enc.h"



/*
   Função: menu_opt

   Descrição:
   Esta função processa os comandos recebidos pelo usuário e executa as operações correspondentes, interagindo com outras funções do sistema.

   Parâmetros:
   - in_str: Uma string contendo o comando de entrada fornecido pelo usuário.
   - sockets: Um array de inteiros que representa os sockets de conexão.
   - Nodei: Um ponteiro para a estrutura Node que representa o nó atual.
   - ring: Uma string que armazena o identificador do anel.
   - id: Uma string que armazena o ID do nó atual.
   - regUDP: Uma string que armazena a porta UDP registrada.
   - regIP: Uma string que armazena o endereço IP registrado.
   - Nodesuc: Um ponteiro para a estrutura Node que representa o nó sucessor direto.
   - Nodesucsuc: Um ponteiro para a estrutura Node que representa o segundo nó sucessor direto.
   - sucfd: Um ponteiro inteiro para o socket do nó sucessor.
   - prefd: Um ponteiro inteiro para o socket do nó predecessor.
   - chordfd: Um ponteiro inteiro para o socket do nó de corda.
   - chords: Um array de inteiros que representa os sockets dos nós de cordas.
   - tabela_enc: Um ponteiro para a estrutura DataFrame que representa a tabela de encaminhamento.
   - tabela_cc: Um ponteiro para a estrutura DataFrame que representa a tabela de caminho mais curto.
   - tabela_exp: Um ponteiro para a estrutura DataFrame que representa a tabela de expedição.
   - rota: Um ponteiro para a estrutura Routes que representa uma rota na rede.

   Retorno:
   - Retorna 0 se o comando indica a saída do programa.
   - Retorna 1 se a operação foi realizada com sucesso ou se o comando é válido.
*/
int menu_opt(char in_str[], int sockets[], struct Node *node, char *ring, char *id, char *regUDP, char *regIP, struct Node *nodesuc, struct Node *nodesucsuc, int *sucid, int *prefd, int *chordfd, int chords[], DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp, Routes *rota);



#endif