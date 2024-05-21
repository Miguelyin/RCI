#ifndef PROTOCOLO_TOP_H
#define PROTOCOLO_TOP_H


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
#include "TCP_Client.h"



/*
   Função: join

   Descrição:
   Esta função executa a operação de join, onde um nó tenta entrar em um anel existente ou cria um novo anel se ainda não houver.

   Parâmetros:
   - node: Um ponteiro para a estrutura Node que representa o próprio nó.
   - ring: Uma string que representa o identificador do anel ao qual o nó está tentando ingressar.
   - id: Uma string que representa o identificador único do nó.
   - regUDP: Uma string contendo as informações de registro UDP.
   - regIP: Uma string contendo as informações de endereço IP do registro.
   - Nodesuc: Um ponteiro para a estrutura Node que representa o sucessor direto do nó.
   - Nodesucsuc: Um ponteiro para a estrutura Node que representa o segundo sucessor do nó.
   - sucfd: Um ponteiro inteiro para o socket do sucessor direto do nó.
   - sockets_fds: Um array de inteiros contendo os sockets dos nós.
   - tabela_enc: Um ponteiro para a estrutura DataFrame que representa a tabela de encaminhamento.
   - tabela_cc: Um ponteiro para a estrutura DataFrame que representa a tabela de caminho mais curto.
   - tabela_exp: Um ponteiro para a estrutura DataFrame que representa a tabela de expedição.
   - rota: Um ponteiro para a estrutura Routes que representa uma rota.

   Comportamento:
   - Envia uma mensagem UDP para obter informações sobre os nós no anel.
   - Analisa a resposta para determinar o modo de join e o nó com o qual fazer join.
   - Realiza as operações necessárias de acordo com o modo de join:
     - Quando o anel está vazio, configura o nó como seu próprio sucessor e predecessor.
     - Quando o anel já contém nós, configura o nó com o sucessor direto e o segundo sucessor, e envia mensagens TCP para atualizar as conexões.
*/
void join(struct Node *node, char *ring, char *id, char *regUDP, char *regIP, struct Node *nodesuc, struct Node *nodesucsuc, int *sucfd, int sockets_fds[], DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp, Routes *rota);


/*
   Função: direct_join

   Descrição:
   Esta função executa a operação de join direto, onde um nó ingressa diretamente em um anel existente sem necessidade de obter informações prévias sobre os nós no anel.

   Parâmetros:
   - node: Um ponteiro para a estrutura Node que representa o próprio nó.
   - id: Uma string que representa o identificador único do nó.
   - Nodesuc: Um ponteiro para a estrutura Node que representa o sucessor direto do nó.
   - Nodesucsuc: Um ponteiro para a estrutura Node que representa o segundo sucessor do nó.
   - sucfd: Um ponteiro inteiro para o socket do sucessor direto do nó.
   - sockets_fds: Um array de inteiros contendo os sockets dos nós.

   Comportamento:
   - Configura o identificador do nó.
   - Configura o sucessor direto do nó.
   - Estabelece uma conexão TCP com o sucessor direto.
   - Atualiza os sockets dos nós.
   - Recebe e processa a resposta do sucessor direto.
   - Configura o segundo sucessor do nó.
*/
void direct_join(struct Node *node, char* id, struct Node *Nodesuc, struct Node *Nodesucsuc, int *sucfd, int sockets_fds[], DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp, Routes *rota);

/*
   Função: Chord

   Descrição:
   Esta função implementa a criação de uma corda em um anel Chord. Uma corda é uma conexão direta entre dois nós no anel.

   Parâmetros:
   - Nodei: Um ponteiro para a estrutura Node que representa o próprio nó.
   - ring: Uma string que representa o identificador do anel Chord.
   - regUDP: Uma string que representa o registro UDP.
   - regIP: Uma string que representa o registro IP.
   - chordfd: Um ponteiro inteiro para o socket da corda.
   - sockets_fds: Um array de inteiros contendo os sockets dos nós.
   - chords: Um array de inteiros que mapeia identificadores de nós para seus respectivos sockets.

   Comportamento:
   - Verifica se já existe uma corda estabelecida. Se sim, exibe uma mensagem de erro e retorna.
   - Cria uma mensagem para solicitar a lista de nós no anel.
   - Envia a mensagem e aguarda a resposta contendo a lista de nós.
   - Analisa a resposta para determinar a viabilidade de criar uma corda.
   - Se for viável, extrai as informações necessárias do nó para estabelecer a corda.
   - Estabelece uma conexão TCP com o nó alvo.
   - Atualiza os sockets dos nós e o mapeamento de identificadores para sockets.
   - Envia uma mensagem para estabelecer a corda.
*/
void Chord(struct Node *Nodei, char *ring, char *regUDP, char *regIP, int *chordfd, int sockets_fds[], int chords[], DataFrame *tabela_cc);

/*
   Função: Chord_to

   Descrição:
   Esta função implementa a criação de uma corda em um anel Chord para um nó específico.

   Parâmetros:
   - Nodei: Um ponteiro para a estrutura Node que representa o próprio nó.
   - chordfd: Um ponteiro inteiro para o socket da corda.
   - sockets_fds: Um array de inteiros contendo os sockets dos nós.
   - chords: Um array de inteiros que mapeia identificadores de nós para seus respectivos sockets.
   - tempid: Uma string que representa o identificador do nó alvo para a corda.
   - tempIP: Uma string que representa o endereço IP do nó alvo.
   - tempTCP: Uma string que representa a porta TCP do nó alvo.

   Comportamento:
   - Verifica se já existe uma corda estabelecida. Se sim, exibe uma mensagem de erro e retorna.
   - Cria uma mensagem para solicitar a criação da corda para o nó alvo.
   - Estabelece uma conexão TCP com o nó alvo.
   - Atualiza os sockets dos nós e o mapeamento de identificadores para sockets.
   - Envia uma mensagem para estabelecer a corda.
*/
void Chord_to(struct Node *Nodei, int *chordfd, int sockets_fds[], int chords[], char *tempid, char *tempIP, char *tempTCP);


/*
   Função: remove_chord

   Descrição:
   Esta função implementa a remoção de uma corda previamente estabelecida em um anel.

   Parâmetros:
   - chordfd: Um ponteiro inteiro para o socket da corda.
   - sockets_fds: Um array de inteiros contendo os sockets dos nós.
   - chords: Um array de inteiros que mapeia identificadores de nós para seus respectivos sockets.

   Comportamento:
   - Verifica se uma corda foi previamente estabelecida. Se não, exibe uma mensagem de erro e retorna.
   - Remove o socket da corda do array de sockets dos nós.
   - Remove a referência da corda do array que mapeia identificadores de nós para sockets.
   - Fecha o socket da corda.
   - Atualiza o valor do ponteiro do socket da corda para indicar que nenhuma corda está ativa.
*/
void remove_chord(int *chordfd, int sockets_fds[], int chords[], int *sucfd, int *prefd, struct Node *Nodei, DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp);

int Keep_ring(struct Node *Nodei, struct Node *Nodesuc, struct Node *Nodesucsuc, struct Node *Nodepre, int sockets[], int i, int prefd, char *prt_str, char *ipstr, char *port_emissor, int sucfd);


/*
   Função: show_topology

   Descrição:
   Esta função exibe informações sobre a topologia do anel Chord, incluindo detalhes sobre o próprio nó, seu sucessor, segundo sucessor, predecessor e vizinhos da corda.

   Parâmetros:
   - node: Um ponteiro para a estrutura do nó atual.
   - chords: Um array de inteiros que mapeia identificadores de nós para sockets de cordas.

   Comportamento:
   - Exibe informações detalhadas sobre o próprio nó, seu sucessor, segundo sucessor e predecessor.
   - Itera sobre o array de cordas para encontrar e exibir os identificadores dos nós que são vizinhos da corda.
   - Se não houver cordas estabelecidas, exibe uma mensagem indicando que não há cordas.
*/
void show_topology(struct Node *node, int chords[]);

/*
   Função: leave

   Descrição:
   Esta função é responsável por realizar a operação de saída do nó atual do anel Chord.

   Parâmetros:
   - node: Um ponteiro para a estrutura do nó atual.
   - ring: O identificador do anel.
   - regUDP: Porta UDP para registo.
   - regIP: Endereço IP para registo.
   - sockets: Um array de sockets TCP.
   - chords: Um array de inteiros que mapeia identificadores de nós para sockets de cordas.
   - sucfd: Um ponteiro para o socket do sucessor.
   - prefd: Um ponteiro para o socket do predecessor.
   - tabela_enc: Um ponteiro para a tabela de encaminhamento.
   - tabela_cc: Um ponteiro para a tabela de caminho mais curto.
   - tabela_exp: Um ponteiro para a tabela de expedição.

   Comportamento:
   - Envia uma mensagem de desregistro (UNREG) para o servidor de nós no anel Chord.
   - Define o predecessor, sucessor e segundo sucessor do nó como ele mesmo, indicando que o nó está saindo do anel.
   - Fecha os sockets TCP abertos pelo nó.
   - Limpa as entradas das tabelas de encaminhamento, caminho mais curto e expedição.
   - Define os ponteiros de sockets do predecessor e sucessor como -1.
*/
void leave(struct Node *node, char *ring, char *regUDP, char *regIP, int sockets[], int chords[], int *sucfd, int *prefd, DataFrame *tabela_enc, DataFrame *tabela_cc, DataFrame *tabela_exp);

#endif
