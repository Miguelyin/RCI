#include "../include/TCP_Server.h"
#include <stdio.h>

int fd1,errcode1, newfd1;
ssize_t n1;
socklen_t addrlen1;
struct addrinfo hints1,*res1;
struct sockaddr_in addr1;
char buffer[128];


int cria_socket_principal(char *PORT){

    fd1=socket(AF_INET,SOCK_STREAM,0); //TCP socket
    if (fd1==-1) exit(1); //error
    
    memset(&hints1,0,sizeof hints1);
    hints1.ai_family=AF_INET; //IPv4
    hints1.ai_socktype=SOCK_STREAM; //TCP socket
    hints1.ai_flags=AI_PASSIVE;

    errcode1=getaddrinfo(NULL,PORT,&hints1,&res1);
    if((errcode1)!=0)/*error*/exit(1);
    
    n1=bind(fd1,res1->ai_addr,res1->ai_addrlen);
    if(n1==-1) /*error*/ exit(1);

    if(listen(fd1,5)==-1)/*error*/exit(1);

    freeaddrinfo(res1);
    return fd1;
}