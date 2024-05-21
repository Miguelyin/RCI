#ifndef PROTOCOLO_CHAT_H
#define PROTOCOLO_CHAT_H


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "UDP_Client.h"

#include "protocolo_enc.h"



/*
   Função: chat

   Descrição:
   Esta função prepara e envia uma mensagem de chat para o destino especificado.

   Parâmetros:
   - buffer: Uma string contendo a mensagem de chat a ser enviada.
   - dest: Uma string que representa o identificador do destinatário da mensagem.
   - Nodei: Um ponteiro para a estrutura Node que representa o nó atual.
   - sucfd: Um ponteiro inteiro para o socket do nó sucessor.
   - prefd: Um ponteiro inteiro para o socket do nó predecessor.
   - chords: Um array de inteiros que representa os sockets dos nós de acordes.
   - tabela_exp: Um ponteiro para a estrutura DataFrame que representa a tabela de expedição.

   Comportamento:
   - Constrói a mensagem de chat a ser enviada no formato "CHAT id_origem id_destino mensagem".
   - Obtém o vizinho mais próximo do destino na tabela de expedição.
   - Verifica se o vizinho é o predecessor, sucessor direto ou outro nó na rede.
   - Envia a mensagem para o vizinho apropriado.
*/
void chat(char* buffer, char* dest, struct Node *Nodei, int *sucfd, int *prefd, int chords [], DataFrame *tabela_exp);


#endif