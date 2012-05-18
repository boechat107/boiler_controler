#ifndef COMUNICACAO_UDP
#define COMUNICACAO_UDP


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>


typedef struct sockaddr_in Sockaddr_in;
typedef struct hostent Hostent;
typedef struct in_addr In_addr;
typedef struct sockaddr Sockaddr;

typedef struct udpCom
{
    int localSocket;
    Sockaddr_in *destAddr;
}UdpCom;


int new_localSocket(void);

Sockaddr_in* new_destAddr(char* dest, int destPort);

int send_msg(UdpCom* udpcom, char *msg);

int recv_msg(UdpCom* udpcom, char *buffer, int bufferSize);

UdpCom* init_udpCom(char* dest, char* port);

void delete_udpCom(UdpCom* udpcom);

#endif
