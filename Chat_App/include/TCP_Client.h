#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include "../include/protocolo_top.h"

int send_and_receive_TCP(char *message, char *response, int fd2);

int cria_socket_Cliente(char *ip, char*Port);

int send_route_msg(char *message, int fd2);

void close_TCP(int sockets[]);

#endif