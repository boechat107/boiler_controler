#ifndef TAREFAS
#define TAREFAS

#include "comunicacao_udp.h"		/* UdpCom */
#include "monitor_concorrencia.h"	/* PthreadAcess, SharedVars */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sched.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

/* ================================================================== 
 * Definicao de constantes / Configuracao
 * ================================================================== */
#define PERIODCOMM 100000   /* Periodo de execucao da tarefa de comunicacao com a caldeira = 100000us */
#define PERIODTEMP 150000   /* Periodo de execucao da tarefa de controle de temp da caldeira = 150000us */
#define PERIODLEVEL 200000   /* Periodo de execucao da tarefa de controle de nivel da caldeira = 200000us */
#define MAX_VALVE 100	    /* Saturacao maxima da valvula Ni */
#define MAX_LEVEL 3	    /* Nivel maximo permitido - 3 m */
#define MIN_LEVEL 0.1	    /* Nivel minimo permitido */
#define SEC_MARGIN 0.15	    /* Margem de segunrancao = 15% */
#define DEFAULT_TEMP_REF 30 /* Temperatura default de referencia */
#define DEFAULT_LEVEL_REF 2 /* Nivel default de referencia */
#define TKP 500		    /* Ganho proporcional - controle temperatura */
#define LKP 100		    /* Ganho proporcional - controle de nivel */


/* ==================================================================
 * Definicao de novas estruturas de dados
 * ================================================================== */


typedef struct paramComm
{
    SharedVars* vars;
    PthreadAcess* pth;
    UdpCom* udpcom;
}ParamComm;


typedef struct paramLog
{
    SharedVars* vars;
    BufferControl* buffer;
    PthreadAcess* pth;
}ParamLog;


typedef struct paramGeneral
{
    SharedVars* vars;
    PthreadAcess* pth;
}ParamGeneral;


/* ==================================================================
 * Renomeando estruturas
 * ================================================================== */
typedef struct timespec Timespec;
typedef struct timeval Timeval;


/* ==================================================================
 * Rotinas/Funcoes
 * ================================================================== */
void start_tasks(char* addr, char* port);
long time_now(long base);

void* caldeiraComm(void* parameters);		/* thread */
void* interface(void* parameters);		/* thread */  
void* temperatureControl(void* parameters);	/* thread */
void* levelControl(void* parameters);		/* thread */
void* logWriter(void* parameters);		/* thread */
void* levelAlarm(void* parameters);		/* thread */

#endif
