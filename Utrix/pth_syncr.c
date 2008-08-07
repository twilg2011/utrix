#include "pth_syncr.h"


pth_mutex_t* list_mux=NULL;/*In realtà inizializzata alla creazione della libreria*/


/*pthread_mutex_init:Inizializza un mutex in base agli attributi(per ipotesi solo NULL ora)
@param: mutex: il mutex da inizializzare
	attr: gli attributi del mutex
@return:OK se è andato tutto bene, altrimenti un errore
@error:EINVAL se il valore del mutex è sbagliato
       EAGAIN se non ho più spazio in memoria 
       EBUSY se il mutex è gia inizializzato*/
int pthread_mutex_int(pthread_mutex_t* mutex, const pthread_mutexattr_t * attr){
pthread_initialize();
	if(!mutex)
		return EINVAL;
	/*Prevedo attr NULL,ipotesi restrittiva*/
	if(attr)
		return EINVAL;
	if(mutex->init && !mutex->mux)/*Inizializzata globalmente*/
	{
		errno=0;
		mutex->mux=(pth_mutex_t*)malloc(sizeof(pth_mutex_t));
		if(!mutex->mux)
			return errno;
	}

	else if(!mutex->init && !mutex->mux)/*da inizializzare*/
	{
		mutex->mux=(pth_mutex_t*)malloc(sizeof(pth_mutex_t));
		if(!mutex->mux)
			return errno;
	}
/*Già inizializzata*/
	if(mutex->mux->state!=NO_INIT)
		return EBUSY;
/*Inizializzo*/
	mutex->mux->state=INIT;
	mutex->mux->val=0;
	mutex->mux->prev=NULL;
	mutex->mux->next=list_mux;
	list_mux->prev=mutex->mux;
	list_mux=mutex->mux;
	

	return OK;
}

/*pthread_mutex_destroy:Distrugge un mutex dalla lista.
@param: mutex: il mutex da inizializzare
@return:OK se è andato tutto bene, altrimenti un errore
@error:EINVAL se il valore del mutex è sbagliato
       EBUSY se il mutex è bloccato*/
int pthread_mutex_destroy(pthread_mutex_t* mutex){
pthread_initialize();
	if(!mutex)
		return EINVAL;
	if(!mutex->mux)
		return EINVAL;
	if(mutex->mux->state==LOCK)
		return EBUSY;
	if(mutex->mux->prev==NULL)/*E' in cima*/
	{	mutex->mux->next->prev=NULL;
		list_mux=mutex->mux->next;
	}	
	else 
		if(mutex->mux->next==NULL)/*E' in fondo*/
			mutex->mux->prev->next=NULL;
		else/*E' in mezzo*/
		{
			mutex->mux->next->prev=mutex->mux->prev;
			mutex->mux->prev->next=mutex->mux->next;
		}
	free(mutex->mux);
	mutex->mux=NULL;
	mutex->mux->state=DESTROY;
	return OK;
}

/*pthread_mutex_lock:Eseguo una lock su un mutex 
@param: mutex: il mutex da inizializzare
@return:OK se è andato tutto bene, altrimenti un errore
@error:EINVAL se il valore del mutex è sbagliato
       EDEADLK se il proprietario del mutex esegue una lock su un mutex già bloccato da lui.*/
int pthread_mutex_lock(pthread_mutex_t *mutex){
pthread_initialize();
/*Se introduco gli attributi qui qualcosa cambia*/
	if(!mutex)
		return EINVAL;
	if(!mutex->mux)
		return EINVAL;
	if((mutex->mux->own==ESECUTION_TID)&&(mutex->mux->state==LOCK))
		return EDEADLK;
	lock(mutex->mux->val);
	mutex->mux->state=LOCK;

	mutex->mux->own= ESECUTION_TID;
	return OK;

}
/*pthread_mutex_unlock:Eseguo una unlock su un mutex 
@param: mutex: il mutex da inizializzare
@return:OK se è andato tutto bene, altrimenti un errore
@error:EINVAL se il valore del mutex è sbagliato
       EPERM se si richiede una unlock su un mutex che non appartiene al thread che esegue l'operazione.*/
int pthread_mutex_unlock(pthread_mutex_t *mutex){
pthread_initialize();
/*Se introduco gli attributi qui qualcosa cambia*/
	if(!mutex)
		return EINVAL;
	if(!mutex->mux)
		return EINVAL;
	if(mutex->mux->own!=ESECUTION_TID)
		return EPERM;
	unlock(mutex->mux->val);
	mutex->mux->state=INIT;
	return OK;

}








