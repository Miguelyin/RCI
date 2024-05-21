#ifndef SELECT_H
#define SELECT_H

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "protocolo_top.h"
#include "protocolo_enc.h"

/*
    Esta função é responsável por executar o select com base no estado atual do nó e nos parâmetros fornecidos.
    
    Parâmetros:
        - node: Ponteiro para a estrutura do nó atual.
        - ring: String representando o identificador do anel.
        - id: String representando o identificador do nó.
        - regUDP: String representando o registro UDP do nó.
        - regIP: String representando o registro IP do nó.
        - nodepre: Ponteiro para o nó predecessor.
        - nodesuc: Ponteiro para o nó sucessor.
        - nodesucsuc: Ponteiro para o segundo nó sucessor.
        - tabela_enc: Ponteiro para o dataframe representando a tabela de encaminhamento.
        - tabela_cc: Ponteiro para o dataframe representando a tabela de controle de congestionamento.
        - tabela_exp: Ponteiro para o dataframe representando a tabela de expedição.
        - rota: Ponteiro para a estrutura de rota.
        
    Retorno:
        - Retorna um inteiro indicando a ação selecionada. Os valores de retorno possíveis são definidos pelo contexto da aplicação e devem ser documentados adequadamente na especificação da função.
*/
int func_select(struct Node *node, char *ring, char *id, char *regUDP, char *regIP, struct Node *nodepre, struct Node *nodesuc, struct Node *nodesucsuc, DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp, Routes *rota);

#endif