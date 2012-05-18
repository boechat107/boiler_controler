#include "tarefas.h"

/* ================================================================== *
 * Inicia tarefas
 * ================================================================== */
void start_tasks(char* addr, char* port)
{
    UdpCom *udpcom;
    SharedVars sharedVars;
    BufferControl buffer;
    PthreadAcess pth;
    ParamComm commData;
    ParamLog logData;
    ParamGeneral genData;
    pthread_t interface_id;
    pthread_t comm_id;
    pthread_t tempControl_id;
    pthread_t levelControl_id;
    pthread_t logWriter_id;
    pthread_t levelAlarm_id;
    char input[10];
    char stop = 'n';
    int opmenu;
    double newValue;

    /* inicializacao das estruturas de dados */
    init_monitor(&pth, &sharedVars, &buffer);
    udpcom = init_udpCom(addr, port);
    sharedVars.Tref = DEFAULT_TEMP_REF;
    sharedVars.Href = DEFAULT_LEVEL_REF;

    genData.vars = &sharedVars;
    genData.pth = &pth;

    commData.vars = &sharedVars;
    commData.pth = &pth;
    commData.udpcom = udpcom;

    logData.vars = &sharedVars;
    logData.buffer = &buffer;
    logData.pth = &pth;

    /* criacao das threads */
    pthread_create(&interface_id, NULL, &interface, &logData);
    pthread_create(&comm_id, NULL, &caldeiraComm, &commData);
    pthread_create(&tempControl_id, NULL, &temperatureControl, &genData);
    pthread_create(&levelControl_id, NULL, &levelControl, &genData);
    pthread_create(&logWriter_id, NULL, &logWriter, &logData);
    pthread_create(&levelAlarm_id, NULL, &levelAlarm, &genData);

    /* Leitura de comandos via teclado */
    while(stop != 'y')
    {
	scanf("%s", input);
	opmenu = atoi(input);
	switch(opmenu)
	{
	    case 1:
		{
		    setscreen(&sharedVars, &pth, 1);
		    scanf("%s", input);
		    newValue = strtod(input, NULL);
		    setTref(&sharedVars, &pth, newValue);
		    setscreen(&sharedVars, &pth, 0);
		    break;
		}

	    case 2:
		{
		    setscreen(&sharedVars, &pth, 2);
		    scanf("%s", input);
		    newValue = strtod(input, NULL);
		    setHref(&sharedVars, &pth, newValue);
		    setscreen(&sharedVars, &pth, 0);
		    break;
		}

	    case 3:
		{
		    setstop_threads(&sharedVars, &pth, 'y');
		    stop = 'y';
		    break;
		}
	    default:
		{}
	}
    }

    /* Acorda threads para finalizacao */
    wakeupThreads(&pth);

    /* Aguarda pelo termino das threads */
    pthread_join(interface_id, NULL);
    pthread_join(comm_id, NULL);
    pthread_join(tempControl_id, NULL);
    pthread_join(levelControl_id, NULL);
    pthread_join(logWriter_id, NULL);
    pthread_join(levelAlarm_id, NULL);

    /* detrutor da estrutura UdpCom */
    delete_udpCom(udpcom);
}



/* ==================================================================
 * Retorna o instante, em microsegundos, relativo a uma base
 * ================================================================== */
long time_now(long base)
{
    Timeval tval;
    long tnow;

    /* Unidade = microsegundos */
    gettimeofday(&tval, NULL);
    tnow = ((tval.tv_sec * 1000000) + tval.tv_usec);

    return tnow - base;
}


/* ================================================================== */
/* Rotina periodica de comunicacao com a caldeira */
/* ================================================================== */
void* caldeiraComm(void* parameters)
{
    Timespec ts, tsr;
    ParamComm *p;
    long base;
    unsigned int i = 0;
    char msg[15];
    char* msg_aux;
    char stop = 'n';
    double newQ, newNi;

    /* Parametros especificos desta rotina */
    p = (ParamComm *)parameters;

    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    tsr.tv_sec = 0;
    tsr.tv_nsec = 0;
    sleep(1);		    /* Para sincronizar com interrupcoes de timer */
    base = time_now(0);	    /* Tempo inicial da tarefa */

    while(stop != 'y')
    {
	if (i < UINT_MAX)
	    i++;
	else
	{
	    i = 1;
	    base = time_now(0);
	}

	/* Temporizacao da tarefa */
	ts.tv_nsec = (i*PERIODCOMM - time_now(base)) * 1000; /* nanosegundos */
	nanosleep(&ts, &tsr);

	/* Leitura de dados */
	sprintf(msg, "st-0");		/* Temperatura T */
	send_msg(p->udpcom, msg);
	recv_msg(p->udpcom, msg, 15);
	msg_aux = strtok(msg, "-");
	msg_aux = strtok(NULL, "");
	setT(p->vars, p->pth, atof(msg_aux));

	sprintf(msg, "sta0");		/* Temperatura Ta */
	send_msg(p->udpcom, msg);
	recv_msg(p->udpcom, msg, 15);
	msg_aux = strtok(msg, "a");
	msg_aux = strtok(NULL, "");
	setTa(p->vars, p->pth, atof(msg_aux));

	sprintf(msg, "sti0");		/* Temperatura Ti */
	send_msg(p->udpcom, msg);
	recv_msg(p->udpcom, msg, 15);
	msg_aux = strtok(msg, "i");
	msg_aux = strtok(NULL, "");
	setTi(p->vars, p->pth, atof(msg_aux));

	sprintf(msg, "sno0");		/* Nivel No */
	send_msg(p->udpcom, msg);
	recv_msg(p->udpcom, msg, 15);
	msg_aux = strtok(msg, "o");
	msg_aux = strtok(NULL, "");
	setNo(p->vars, p->pth, atof(msg_aux));

	sprintf(msg, "sh-0");		/* Nivel H */
	send_msg(p->udpcom, msg);
	recv_msg(p->udpcom, msg, 15);
	msg_aux = strtok(msg, "-");
	msg_aux = strtok(NULL, "");
	setH(p->vars, p->pth, atof(msg_aux));

	/* Envio de dados */
	readQ(p->vars, p->pth, &newQ);
	readNi(p->vars, p->pth, &newNi);
	readstop_threads(p->vars, p->pth, &stop);

	sprintf(msg, "aq-%f", newQ);
	send_msg(p->udpcom, msg);
	recv_msg(p->udpcom, msg, 15);		/* unused */

	sprintf(msg, "ani%f", newNi);
	send_msg(p->udpcom, msg);
	recv_msg(p->udpcom, msg, 15);		/* unused */
    }
	
    return NULL;
}



/* ================================================================== 
 * Controle da temperatura
 * ================================================================== */
void* temperatureControl(void* parameters)
{
    Timespec ts, tsr;
    ParamGeneral *p;
    long base;
    unsigned int i = 0;
    char stop = 'n';
    double temp, tempRef, heater, errorRef;

    /* Parametros especificos desta rotina */
    p = (ParamGeneral *)parameters;

    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    tsr.tv_sec = 0;
    tsr.tv_nsec = 0;
    sleep(1);
    base = time_now(0);

    while(stop != 'y')
    {
	if (i < UINT_MAX)
	    i++;
	else
	{
	    i = 1;
	    base = time_now(0);
	}

	/* Temporizacao da tarefa */
	ts.tv_nsec = (i*PERIODTEMP - time_now(base)) * 1000; /* nanosegundos */
	nanosleep(&ts, &tsr);

	/* Leitura de variaveis compartilhadas */
	readstop_threads(p->vars, p->pth, &stop);
	readT(p->vars, p->pth, &temp);
	readTref(p->vars, p->pth, &tempRef);

	/* Calculo do valor de atuacao do aquecedor */
	errorRef = tempRef - temp;
	heater = TKP * errorRef;
	if (heater < 0)
	    heater = 0;

	/* Escrita de variaveis compartilhadas */
	setQ(p->vars, p->pth, heater);
    }

    return NULL;
}


/* ================================================================== 
 * Controle do nivel
 * ================================================================== */
void* levelControl(void* parameters)
{
    Timespec ts, tsr;
    ParamGeneral *p;
    long base;
    unsigned int i = 0;
    char stop = 'n';
    double level, levelRef, valve, errorRef, minLevel, maxLevel;

    /* Parametros especificos desta rotina */
    p = (ParamGeneral *)parameters;
    minLevel = (1+SEC_MARGIN)*MIN_LEVEL;
    maxLevel = (1-SEC_MARGIN)*MAX_LEVEL;

    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    tsr.tv_sec = 0;
    tsr.tv_nsec = 0;
    sleep(1);
    base = time_now(0);

    while(stop != 'y')
    {
	if (i < UINT_MAX)
	    i++;
	else
	{
	    i = 1;
	    base = time_now(0);
	}

	/* Temporizacao da tarefa */
	ts.tv_nsec = (i*PERIODLEVEL - time_now(base)) * 1000; /* nanosegundos */
	nanosleep(&ts, &tsr);

	/* Leitura de variaveis compartilhadas */
	readstop_threads(p->vars, p->pth, &stop);
	readH(p->vars, p->pth, &level);
	readHref(p->vars, p->pth, &levelRef);

	/* Verificao dos limites de nivel permitidos */
	if (level <= minLevel || level >= maxLevel)
	    wakeuplevAlarm(p->pth);

	/* Calculo do valor de atuacao do aquecedor */
	errorRef = levelRef - level;
	valve = LKP * errorRef;
	if (valve < 0)
	    valve = 0;
	if (valve > MAX_VALVE)
	    valve = MAX_VALVE;

	/* Escrita de variaveis compartilhadas */
	setNi(p->vars, p->pth, valve);
    }

    return NULL;
}


/* ================================================================== 
 * Interface com usuario
 * ================================================================== */
void* interface(void* parameters)
{
    ParamLog* p;
    SharedVars v;
    char screen;
    char stop = 'n';

    p = (ParamLog*)parameters;

    while(stop != 'y'){
	system("clear");
	readscreen(p->vars, p->pth, &screen);
	puts("==================================================================\n\n\n"
		"\t\tInterface de controle da caldeira\n\n\n"
		"==================================================================\n");
	switch(screen)
	{
	    case 0:
		{
		    /* Leitura das variaveis */
		    readT(p->vars, p->pth, &v.T);
		    readTref(p->vars, p->pth, &v.Tref);
		    readTa(p->vars, p->pth, &v.Ta);
		    readTi(p->vars, p->pth, &v.Ti);
		    readH(p->vars, p->pth, &v.H);
		    readHref(p->vars, p->pth, &v.Href);
		    readQ(p->vars, p->pth, &v.Q);
		    readNi(p->vars, p->pth, &v.Ni);
		    readNo(p->vars, p->pth, &v.No);
		    writeBuffer(&v, p->buffer, p->pth);

		    /* Imprime status da caldeira */
		    /*printf("\n\t\t** Status da caldeira **\n\n"
			    "\tTemperatura agua = %f C\n"
			    "\tTemperatura (referencia) = %f C\n"
			    "\tTemperatura agua em Ni = %f C\n"
			    "\tTemperatura ambiente = %f C\n"
			    "\tNivel de agua = %f m\n"
			    "\tNivel (referencia) = %f m\n\n"
			    "\tAquecedor = %f J/s\n"
			    "\tValvula de entrada = %f Kg/s\n\n"
			    "\tValvula de saida = %f Kg/s\n\n"*/

		    printf("\n\t\t** Status da caldeira **\n\n"
			    "\tT =\t%f C\n"
			    "\tTref =\t%f C\n"
			    "\tTi =\t%f C\n"
			    "\tTa =\t%f C\n"
			    "\tH =\t%f m\n"
			    "\tHref =\t%f m\n"
			    "\tQ =\t%f J/s\n"
			    "\tNi =\t%f Kg/s\n"
			    "\tNo =\t%f Kg/s\n\n"
			    , v.T, v.Tref, v.Ti, v.Ta, v.H, v.Href, v.Q, v.Ni, v.No);

		    /* Imprime menu de navegacao */
		    puts("\nOpcoes:\n"
			    "1) Alterar temperatura de referencia para caldeira\n"
			    "2) Alterar nivel de referencia da agua\n"
			    "3) Sair\n\n"
			    "Digite o numero da opcao e pressione ENTER: ");
		    break;
		}

	    case 1:
		{
		    puts("\n\nNovo valor da temperatura de referencia (C): ");
		    break;
		}

	    case 2:
		{
		    puts("\n\nNovo valor do nivel de referencia (m): ");
		    break;
		}

	    default:{}
	}
	
	sleep(1);
	readstop_threads(p->vars, p->pth, &stop);
    }

    return NULL;
}


/* ================================================================== 
 * Tarefa de gravacao de log
 * ================================================================== */
void* logWriter(void* parameters)
{
    ParamLog* p;
    SharedVars v;
    FILE* file;
    char stop = 'n';

    p = (ParamLog*)parameters;

    /* Ponteiro para arquivo de texto */
    file = fopen("execution_log.txt", "w");
    if (file == NULL)
    {
	puts("\n\n\tProblemas ao escrever log em arquivo!!\n\n");
	exit(1);
    }

    /* Primeira mensagem do arquivo */
    fputs("=================================================================="
	    "\n\tLog de execucao do controlador de caldeira\n"
	    "=================================================================="
	    "\n"
	    , file);

    while (stop != 'y')
    {
	v = readBuffer(p->buffer, p->pth);
	readstop_threads(p->vars, p->pth, &stop);
	if (stop == 'y')
	    break;
	fprintf(file, "\tTemperatura agua = %f C\n"
		"\tTemperatura agua (referencia) = %f C\n"
		"\tTemperatura agua da entrada = %f C\n"
		"\tTemperatura ambiente = %f C\n"
		"\tNivel de agua = %f m\n"
		"\tNivel (referencia) = %f m\n"
		"\tAquecedor = %f J/s\n"
		"\tValvula de entrada = %f Kg/s\n"
		"\tValvula de saida = %f Kg/s\n"
		"\n\n"
		, v.T, v.Tref, v.Ti, v.Ta, v.H, v.Href, v.Q, v.Ni, v.No);
    }

    fclose(file);
    return NULL;
}


/* ================================================================== 
 * Thread de alarme de nivel de agua
 * ================================================================== */
void* levelAlarm(void* parameters)
{
    ParamGeneral* p;
    double H;
    char stop = 'n';

    p = (ParamGeneral*)parameters;

    while (stop != 'y')
    {
	data4levAlarm(p->vars, p->pth, &H);
	readstop_threads(p->vars, p->pth, &stop);
	if (stop == 'y')
	    break;

	/* Acao da thread de alarme */
	if (H <= SEC_MARGIN*MIN_LEVEL)
	    puts("\n\n\t** Cuidado! **\n\tNivel de agua proximo do minimo permitido!\n");
	if (H >= SEC_MARGIN*MAX_LEVEL)
	    puts("\n\n\t** Cuidado! **\n\tNivel de agua proximo do maximo permitido!\n");
    }

    return NULL;
}
