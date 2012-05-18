/*
================================================================== 
ANDRE AMBROSIO BOECHAT
DAS-9002
Wed Mar 31 17:58:08 BRT 2010
================================================================== 
*/

#include <stdio.h>
#include "comunicacao_udp.h"
#include "tarefas.h"


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
	puts("\n\tSao necessarios dois argumentos para execucao: endereco do servidor e porta de comunicacao.\n");
	exit(1);
    }

    start_tasks(argv[1], argv[2]);

    return 0;
}
