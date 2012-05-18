#include "monitor_concorrencia.h"

/* ================================================================== 
 * Inicia estruturas do monitor
 * ================================================================== */
void init_monitor(PthreadAcess* pth, SharedVars* vars, BufferControl* buffer)
{
    vars->H = 0;
    vars->Href = 0;
    vars->Ni = 0;
    vars->No = 0;
    vars->Q = 0;
    vars->T = 0;
    vars->Tref = 0;
    vars->Ta = 0;
    vars->Ti = 0;
    vars->stop_threads = 'n';
    vars->screen = 0;

    buffer->bufferRec = 0;
    buffer->bufferRead = 0;
    buffer->ndata = 0;

    pthread_mutex_init(&pth->mQ, NULL);
    pthread_mutex_init(&pth->mNi, NULL);
    pthread_mutex_init(&pth->mT, NULL);
    pthread_mutex_init(&pth->mTref, NULL);
    pthread_mutex_init(&pth->mPert, NULL);
    pthread_mutex_init(&pth->mH, NULL);
    pthread_mutex_init(&pth->mHref, NULL);
    pthread_mutex_init(&pth->mstop_threads, NULL);
    pthread_mutex_init(&pth->mscreen, NULL);
    pthread_mutex_init(&pth->mbuffer, NULL);

    pthread_cond_init(&pth->writeBuffer, NULL);
    pthread_cond_init(&pth->levAlarm, NULL);
}


/* ================================================================== 
 * Ler valor de qualquer variavel
 * ================================================================== */
void readGen(double* sv, pthread_mutex_t* m, double* res)
{
    pthread_mutex_lock(m);
    *res = *sv;
    pthread_mutex_unlock(m);
}


/* ================================================================== 
 * Alterar valor de qualquer variavel
 * ================================================================== */
void setGen(double* sv, pthread_mutex_t* m, double* value)
{
    pthread_mutex_lock(m);
    *sv = *value;
    pthread_mutex_unlock(m);
}


/* ================================================================== 
 * Ler valor da variavel Q
 * ================================================================== */
void readQ(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->Q, &p->mQ, res);
}


/* ================================================================== 
 * Alterar valor da variavel Q
 * ================================================================== */
void setQ(SharedVars* v, PthreadAcess* p, double value)
{
    setGen(&v->Q, &p->mQ, &value);
}


/* ================================================================== 
 * Ler valor da variavel Ni
 * ================================================================== */
void readNi(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->Ni, &p->mNi, res);
}


/* ================================================================== 
 * Alterar valor da variavel Ni
 * ================================================================== */
void setNi(SharedVars* v, PthreadAcess* p, double value)
{
    pthread_mutex_lock(&p->mNi);
    v->Ni = value;
    pthread_mutex_unlock(&p->mNi);
}


/* ================================================================== 
 * Ler valor da variavel No
 * ================================================================== */
void readNo(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->No, &p->mPert, res);
}


/* ================================================================== 
 * Alterar valor da variavel No
 * ================================================================== */
void setNo(SharedVars* v, PthreadAcess* p, double value)
{
    setGen(&v->No, &p->mPert, &value);
}


/* ================================================================== 
 * Ler valor da variavel T
 * ================================================================== */
void readT(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->T, &p->mT, res);
}


/* ================================================================== 
 * Alterar valor da variavel T
 * ================================================================== */
void setT(SharedVars* v, PthreadAcess* p, double value)
{
    setGen(&v->T, &p->mT, &value);
}


/* ================================================================== 
 * Ler valor da variavel Tref
 * ================================================================== */
void readTref(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->Tref, &p->mTref, res);
}


/* ================================================================== 
 * Alterar valor da variavel Tref
 * ================================================================== */
void setTref(SharedVars* v, PthreadAcess* p, double value)
{
    setGen(&v->Tref, &p->mTref, &value);
}


/* ================================================================== 
 * Ler valor da variavel Ta
 * ================================================================== */
void readTa(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->Ta, &p->mPert, res);
}


/* ================================================================== 
 * Alterar valor da variavel Ta
 * ================================================================== */
void setTa(SharedVars* v, PthreadAcess* p, double value)
{
    setGen(&v->Ta, &p->mPert, &value);
}


/* ================================================================== 
 * Ler valor da variavel Ti
 * ================================================================== */
void readTi(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->Ti, &p->mPert, res);
}


/* ================================================================== 
 * Alterar valor da variavel Ti
 * ================================================================== */
void setTi(SharedVars* v, PthreadAcess* p, double value)
{
    setGen(&v->Ti, &p->mPert, &value);
}


/* ================================================================== 
 * Ler valor da variavel H
 * ================================================================== */
void readH(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->H, &p->mH, res);
}


/* ================================================================== 
 * Alterar valor da variavel H
 * ================================================================== */
void setH(SharedVars* v, PthreadAcess* p, double value)
{
    setGen(&v->H, &p->mH, &value);
}


/* ================================================================== 
 * Ler valor da variavel Href
 * ================================================================== */
void readHref(SharedVars* v, PthreadAcess* p, double* res)
{
    readGen(&v->Href, &p->mHref, res);
}


/* ================================================================== 
 * Alterar valor da variavel Href
 * ================================================================== */
void setHref(SharedVars* v, PthreadAcess* p, double value)
{
    setGen(&v->Href, &p->mHref, &value);
}


/* ================================================================== 
 * Ler valor da variavel stop_threads
 * ================================================================== */
void readstop_threads(SharedVars* v, PthreadAcess* p, char* res)
{
    pthread_mutex_lock(&p->mstop_threads);
    *res = v->stop_threads;
    pthread_mutex_unlock(&p->mstop_threads);
}


/* ================================================================== 
 * Alterar valor da variavel stop_threads
 * ================================================================== */
void setstop_threads(SharedVars* v, PthreadAcess* p, char value)
{
    pthread_mutex_lock(&p->mstop_threads);
    v->stop_threads = value;
    pthread_mutex_unlock(&p->mstop_threads);
}


/* ================================================================== 
 * Ler valor da variavel screen
 * ================================================================== */
void readscreen(SharedVars* v, PthreadAcess* p, char* res)
{
    pthread_mutex_lock(&p->mscreen);
    *res = v->screen;
    pthread_mutex_unlock(&p->mscreen);
}


/* ================================================================== 
 * Alterar valor da variavel screen
 * ================================================================== */
void setscreen(SharedVars* v, PthreadAcess* p, char value)
{
    pthread_mutex_lock(&p->mscreen);
    v->screen = value;
    pthread_mutex_unlock(&p->mscreen);
}


/* ================================================================== 
 * Escrever no buffer
 * ================================================================== */
void writeBuffer(SharedVars* vars, BufferControl* buffer, PthreadAcess* p)
{
    pthread_mutex_lock(&p->mbuffer);
    if (buffer->ndata != DATABUFFERSIZE)
    {
	buffer->array[buffer->bufferRec] = *vars;
	buffer->bufferRec = (buffer->bufferRec + 1) % DATABUFFERSIZE;
	buffer->ndata ++;
	pthread_cond_signal(&p->writeBuffer);
    }
    pthread_mutex_unlock(&p->mbuffer);
}


/* ================================================================== 
 * Ler do buffer
 * ================================================================== */
SharedVars readBuffer(BufferControl* buffer, PthreadAcess* p)
{
    SharedVars output;

    pthread_mutex_lock(&p->mbuffer);
   
    if (buffer->ndata == 0)
	pthread_cond_wait(&p->writeBuffer, &p->mbuffer);
    output = buffer->array[buffer->bufferRead];
    buffer->bufferRead = (buffer->bufferRead + 1) % DATABUFFERSIZE;
    buffer->ndata --;

    pthread_mutex_unlock(&p->mbuffer);

    return output;
}


/* ================================================================== 
 * Acorda threads para finalizacao
 * ================================================================== */
void wakeupThreads(PthreadAcess* p)
{
    pthread_cond_signal(&p->writeBuffer);
    pthread_cond_signal(&p->levAlarm);
}


/* ================================================================== 
 * Dispara thread de alarme de nivel
 * ================================================================== */
void wakeuplevAlarm(PthreadAcess* p)
{
    pthread_cond_signal(&p->levAlarm);
}


/* ================================================================== 
 * Acesso a variavel H para a thread de alarme
 * ================================================================== */
void data4levAlarm(SharedVars* v, PthreadAcess* p, double* aH)
{
    pthread_mutex_lock(&p->mH);
    pthread_cond_wait(&p->levAlarm, &p->mH);
    *aH = v->H;
    pthread_mutex_unlock(&p->mH);
}
