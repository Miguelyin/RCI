#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "../include/TCP_Client.h"


int fd2,errcode2;
ssize_t n2;
socklen_t addrlen2;
struct addrinfo hints2,*res2;
struct sockaddr_in addr2;
char buffer2[128];


int cria_socket_Cliente(char *ip, char*PORT){

    fd2=socket(AF_INET,SOCK_STREAM,0); //TCP socket
    if (fd2==-1) exit(1); //error

    

    memset(&hints2,0,sizeof hints2);
    hints2.ai_family=AF_INET; //IPv4
    hints2.ai_socktype=SOCK_STREAM; //TCP socket

    errcode2=getaddrinfo(ip,PORT,&hints2,&res2);
    if(errcode2!=0)/*error*/exit(1);

    n2=connect(fd2,res2->ai_addr,res2->ai_addrlen);
    if(n2==-1)/*error*/exit(1);
    //printf("Aqui\n");

    return fd2;

}

int send_and_receive_TCP(char *message, char *response, int fd2){

    n2=write(fd2,message,strlen(message));
    if(n2==-1)exit(1);

    n2=read(fd2,response,128);
    if(n2==-1)exit(1);

    return 0;

}

int send_route_msg(char *message, int fd2){
    //printf("\n\nSOCKET DO SEND_ROUTE_MSG: %d", fd2);
    n2=write(fd2, message, strlen(message));
    if(n2==-1)exit(1);

    return 0;}



void close_TCP(int sockets[]){
    int i;

    for (i = 1; i < 100; i++) {    //Não vamos fechamos o socket do TCP Server
        if (sockets[i] != -1) {
            close(sockets[i]);
            sockets[i] = -1;
            //printf("Conexão %d fechada com sucesso.\n", i);
        }
    }
}
