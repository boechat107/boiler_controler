#include "comunicacao_udp.h" 


/* ================================================================== */
int new_localSocket(void)
{
    int socket_local;

    socket_local = socket(PF_INET, SOCK_DGRAM, 0);
    if(socket_local < 0)
    {
	perror("socket");
	return -1;
    }
    return socket_local;
}


/* ================================================================== */
Sockaddr_in* new_destAddr(char* dest, int destPort)
{
    Sockaddr_in *sockaddr_serv;
    Hostent *addr_dest;
    In_addr ip_dest;

    if (inet_aton(dest, &ip_dest))
	addr_dest = gethostbyaddr((char* )&ip_dest, sizeof(ip_dest), AF_INET);
    else
	addr_dest = gethostbyname(dest);

    if(addr_dest == NULL)
    {
	printf("Endereco de rede invalido");
	exit(1);
    }

    sockaddr_serv = (Sockaddr_in*)malloc(sizeof(Sockaddr_in));
    memset((char *)sockaddr_serv, 0, sizeof(*sockaddr_serv));
    memcpy(&sockaddr_serv->sin_addr, addr_dest->h_addr_list[0], sizeof(*sockaddr_serv));
    sockaddr_serv->sin_family = AF_INET;
    sockaddr_serv->sin_port = htons(destPort);

    return sockaddr_serv;
}


/* ================================================================== */
/* Envia mensagem ao servidor */
int send_msg(UdpCom* udpcom, char *msg)
{
    if (sendto(udpcom->localSocket, msg, strlen(msg)+1, 0, (Sockaddr *)udpcom->destAddr, sizeof(*udpcom->destAddr)) < 0)
    { 
	perror("sendto");
	return -1;
    }
    return 1;
}


/* ================================================================== */
/* Recebe mensagem do servidor */
int recv_msg(UdpCom* udpcom, char *buffer, int bufferSize)
{
    int bytes_recv;		/* Número de bytes recebidos */

    /* Espera pela msg de resposta do servidor */
    bytes_recv = recvfrom(udpcom->localSocket, buffer, bufferSize, 0, NULL, 0);
    if (bytes_recv < 0)
    {
	perror("recvfrom");
    }

    return bytes_recv;
}


/* ================================================================== */
/* Inicializa a estrutura de dados para comunicacao udp com o servidor */
UdpCom* init_udpCom(char* dest, char* port)
{
    int porti;
    int localSocket;
    Sockaddr_in *sock;
    UdpCom* udpcom;

    udpcom = (UdpCom*)malloc(sizeof(UdpCom));
    localSocket = new_localSocket();
    porti = atoi(port);
    sock = new_destAddr(dest, porti);
    udpcom->localSocket = localSocket;
    udpcom->destAddr = sock;

    return udpcom;
}


/* ================================================================== */
/* Libera a memoria alocada nas estruturas */
void delete_udpCom(UdpCom* udpcom)
{
    free(udpcom->destAddr);
    free(udpcom);
}

