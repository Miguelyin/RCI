#include "../include/UDP_Client.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

int fd,errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints,*res;
struct sockaddr_in addr;


void send_UDP(char *response, char *message, char *regUDP, char *regIP){

    char buffer[300]="";
    

    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(fd==-1) /*error*/exit(1);

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_DGRAM; //UDP socket

    errcode=getaddrinfo(regIP,regUDP,&hints,&res);
    if(errcode!=0) /*error*/ exit(1);



    //envia a mensagem
    n=sendto(fd,message,128,0,res->ai_addr,res->ai_addrlen);
    if(n==-1) /*error*/ exit(1);

    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,300,0,
    (struct sockaddr*)&addr,&addrlen);
    if(n==-1) /*error*/ exit(1);

   
    //obtém a resposta
    strcpy(response, buffer);

    freeaddrinfo(res);
    close(fd);

    

}