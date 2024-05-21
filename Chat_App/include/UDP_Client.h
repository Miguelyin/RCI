#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H


#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>


// Declaration of functions
void send_UDP(char *, char *, char *regUDP, char* regIP);

#endif