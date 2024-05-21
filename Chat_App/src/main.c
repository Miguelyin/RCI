#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/wait.h>

#include "../include/protocolo_enc.h"
#include "../include/protocolo_top.h"
#include "../include/select.h"




int main(int argc, char *argv[]) {
    struct Node Nodei; // Create a struct Node object
    struct Node Nodesuc;
    struct Node Nodesucsuc;
    struct Node Nodepre;

    Routes rota;    //Inicializa o struct das Rotas
    
    //Inicialização das tabelas de routing
    DataFrame tabela_enc = {0}; //Inicializa tudo a 0 na tabela de encaminhamento
    DataFrame tabela_cc = {0};  //Inicializa tudo a 0 na tabela de caminhos mais curtos
    DataFrame tabela_exp = {0}; //Inicializa tudo a 0 na tabela de expedição
    
    tabela_enc.largura = 18; // Definindo a largura para 16 colunas.
    tabela_enc.altura = 18;
    
    tabela_cc.largura = 2; // Definindo a largura para 2 colunas.
    tabela_cc.altura = 18;

    tabela_exp.largura = 2; // Definindo a largura para 2 colunas.
    tabela_exp.altura = 18;


    //Inicialização das variáveis obtidas quando se corre o programa

    char IP[] = "194.210.158.202"; //IP do computador em que se corre a aplicação
    char TCP[] = "8080000"; // TCP PORT do computador em que se corre a aplicação
    char regIP[] = "193.136.138.142"; //IP do servidor de nós
    char regUDP[] = "59000"; // UDP PORT do servidor de nós


    if (argc != 5) {
        printf("Usage: %s arg1 arg2 arg3 arg4\n", argv[0]); // Print usage message
        return 1; // Exit with an error code
    }


    //Leitura dos parâmeteros da invocação do programa COR
    sprintf(IP, "%s", argv[1]);
    sprintf(TCP, "%s", argv[2]);
    sprintf(regIP, "%s", argv[3]);    //char pra char (regIp)
    sprintf(regUDP, "%s", argv[4]);



    //Inicializa os valores do ring e id
    char ring[]= "000";
    char id[]= "00";




    // Initialize the Nodei object with his id and contact (IP and TCP port)
    strcpy(Nodei.id, id);
    strcpy(Nodei.IP, IP);
    strcpy(Nodei.TCP, TCP);

    
    Nodei.predecessor = NULL;
    Nodei.sucessor = NULL;
    Nodei.segundo_sucessor = NULL;
    




    //Função select que permite o programa funcionar ao mesmo tempo que está sempre pronto a receber mensagens TCP e inputs da interface
    func_select(&Nodei, ring, id, regUDP, regIP, &Nodepre, &Nodesuc, &Nodesucsuc, &tabela_enc, &tabela_cc, &tabela_exp, &rota);
    return 0;

}
