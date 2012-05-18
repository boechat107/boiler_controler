#ifndef MONITOR
#define MONITOR

#include <pthread.h>


/* ================================================================== 
 * Configuracoes - Constantes
 * ================================================================== */
#define DATABUFFERSIZE 2	    /* Tamanho do buffer de log */


/* ================================================================== 
 * Estruturas de dados
 * ================================================================== */

typedef struct sharedVars
{
    double Ni;
    double No;
    double Q;
    double T;
    double Tref;
    double Ta;
    double Ti;
    double H;
    double Href;
    char screen;	/* indica o que deve ser impresso no terminal */
    char stop_threads;	/* y-> parar threads */
}SharedVars;


typedef struct bufferControl
{
    SharedVars array[DATABUFFERSIZE];
    int bufferRec;
    int bufferRead;
    int ndata;
}BufferControl;


typedef struct pthreadAcess
{
    pthread_mutex_t mQ;		/* mutex var Q */
    pthread_mutex_t mNi;	/* mutex var Ni */
    pthread_mutex_t mT;		/* mutex var T */
    pthread_mutex_t mTref;	/* mutex var Tref */
    pthread_mutex_t mPert;	/* mutex var Ta, Ti, No */
    pthread_mutex_t mH;		/* mutex var H */
    pthread_mutex_t mHref;	/* mutex var Href */
    pthread_mutex_t mstop_threads;	/* mutex var stop_threads */
    pthread_mutex_t mscreen;	/* mutex var screen */
    pthread_mutex_t mbuffer;	/* mutex para gravacao em arquivo */

    pthread_cond_t writeBuffer;
    pthread_cond_t levAlarm;

    pthread_mutex_t mexc;
    pthread_cond_t ok2writers;
    pthread_cond_t ok2readers;
    short int rcount;			/* numero de leitores */
    short int pendingw;			/* escritores pendentes */
    char busy;				/* 'y'-> escritor ocupado */
}PthreadAcess;

/* ================================================================== */
void init_monitor(PthreadAcess* pth, SharedVars* vars, BufferControl* buffer);

void readGen(double* sv, pthread_mutex_t* m, double* res);
void setGen(double* sv, pthread_mutex_t* m, double* value);

void readQ(SharedVars* v, PthreadAcess* p, double* res);
void setQ(SharedVars* v, PthreadAcess* p, double value);

void readNi(SharedVars* v, PthreadAcess* p, double* res);
void setNi(SharedVars* v, PthreadAcess* p, double value);

void readNo(SharedVars* v, PthreadAcess* p, double* res);
void setNo(SharedVars* v, PthreadAcess* p, double value);

void readT(SharedVars* v, PthreadAcess* p, double* res);
void setT(SharedVars* v, PthreadAcess* p, double value);

void readTref(SharedVars* v, PthreadAcess* p, double* res);
void setTref(SharedVars* v, PthreadAcess* p, double value);

void readTa(SharedVars* v, PthreadAcess* p, double* res);
void setTa(SharedVars* v, PthreadAcess* p, double value);

void readTi(SharedVars* v, PthreadAcess* p, double* res);
void setTi(SharedVars* v, PthreadAcess* p, double value);

void readH(SharedVars* v, PthreadAcess* p, double* res);
void setH(SharedVars* v, PthreadAcess* p, double value);

void readHref(SharedVars* v, PthreadAcess* p, double* res);
void setHref(SharedVars* v, PthreadAcess* p, double value);

void readstop_threads(SharedVars* v, PthreadAcess* p, char* res);
void setstop_threads(SharedVars* v, PthreadAcess* p, char value);

void readscreen(SharedVars* v, PthreadAcess* p, char* res);
void setscreen(SharedVars* v, PthreadAcess* p, char value);

void writeBuffer(SharedVars* vars, BufferControl* buffer, PthreadAcess* p);
SharedVars readBuffer(BufferControl* buffer, PthreadAcess* p);

void wakeuplevAlarm(PthreadAcess* p);
void data4levAlarm(SharedVars* v, PthreadAcess* p, double* aH);
void wakeupThreads(PthreadAcess* p);

#endif
