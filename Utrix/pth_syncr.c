
#include "pth_syncr.h"
#include "pth_errno.h"
#define SEARCH(list,elem) if(elem->prev==NULL)/*E' in cima*/\
			{	elem->next->prev=NULL;\
				list=elem->next;\
			}	\
			else \
				if(elem->next==NULL)/*E' in fondo*/\
					elem->prev->next=NULL;\
				else/*E' in mezzo*/\
				{\
					elem->next->prev=elem->prev;\
					elem->prev->next=elem->next;\
				}




void lock(int* val);
void unlock(int* val);
pth_mutex_t* list_mux=NULL; /*In realtà inizializzata alla creazione della libreria*/
pth_cond_t* list_cond=NULL;


/*pthread_mutex_init:Inizializza un mutex in base agli attributi(per ipotesi solo NULL ora)
@param: mutex: il mutex da inizializzare
	attr: gli attributi del mutex
@return:OK se è andato tutto bene, altrimenti un errore
@error:EINVAL se il valore del mutex è sbagliato
       EAGAIN se non ho più spazio in memoria 
       EBUSY se il mutex è gia inizializzato*/
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t * attr){
pthread_initialize();
	if(!mutex)
		return SETERR(EINVAL);
	/*Prevedo attr NULL,ipotesi restrittiva*/
	if(attr)
		return SETERR(EINVAL);
	if(!mutex->mux){
		if(mutex->init==NOT_INIT)/*Non inizializzata*/
			mutex->init=INIT;
		SETERR(0);
		mutex->mux=(pth_mutex_t*)malloc(sizeof(pth_mutex_t));
		if(!mutex->mux)
			return SETERR(ENOMEM);
		mutex->mux->state=NO_ACTIVE;	
	}
/*Già inizializzata*/
	if(mutex->mux->state!=NO_ACTIVE)
		return SETERR(EBUSY);
/*Inizializzo*/
	mutex->mux->state=ACTIVE;
	mutex->mux->val=0;
	mutex->mux->prev=NULL;
	mutex->mux->next=list_mux;
	list_mux->prev=mutex->mux;
	list_mux=mutex->mux;
	

	return SETERR(OK);
}

/*pthread_mutex_destroy:Distrugge un mutex dalla lista.
@param: mutex: il mutex da inizializzare
@return:OK se è andato tutto bene, altrimenti un errore
@error:EINVAL se il valore del mutex è sbagliato
       EBUSY se il mutex è bloccato*/
int pthread_mutex_destroy(pthread_mutex_t* mutex){
        pthread_initialize();
	if(mutex==NULL)/*Non esiste l'elemento passato*/
		return SETERR(EINVAL);
	if(mutex->mux==NULL)/*Il mutex non è stato inizializzato o è gia stato distrutto*/
		return SETERR(EINVAL);
	if(mutex->mux->state==LOCK)
		return SETERR(EBUSY);
	SEARCH(list_mux,mutex->mux)
	free(mutex->mux);
	mutex->mux=NULL;
	return SETERR(OK);
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
		return SETERR(EINVAL);
	if(!mutex->mux)
		return SETERR(EINVAL);
	if((mutex->mux->own==ESECUTION_TID)&&(mutex->mux->state==LOCK))
		return SETERR(EDEADLK);
	lock(mutex->mux->val);/*<---------------------cosa faccio rimango inattivo o passo il controllo allo scheduler?*/
	mutex->mux->state=LOCK;

	mutex->mux->own= ESECUTION_TID;
	return SETERR(OK);

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
		return SETERR(EINVAL);
	if(!mutex->mux)
		return SETERR(EINVAL);
	if(mutex->mux->own!=ESECUTION_TID)
		return SETERR(EPERM);
	unlock(mutex->mux->val);
	mutex->mux->state=INIT;
	return SETERR(OK);

}



int pthread_cond_init(pthread_cond_t *cond /*,const pthread_condattr_t *attr*/){
	pthread_initialize();
	if(!cond)
		return SETERR(EINVAL);
/*Ipotesi restrittiva
if(attr)
		return EINVAL;
*/
	if(!cond->condition){
		if(cond->init==NOT_INIT)/*Inizializzato da una macro*/
			cond->init=INIT;
			SETERR(0);
			cond->condition=(pth_cond_t*)malloc(sizeof(pth_cond_t));	
			if(!cond->condition)
				return SETERR(ENOMEM);
			cond->condition->state=NO_ACTIVE_COND;

		}
	if(cond->condition->state==ACTIVE_COND)
		 return SETERR(EBUSY);
	cond->condition->state=ACTIVE_COND;
	cond->condition->list_head=NULL;
        cond->condition->list_tail=NULL;
	cond->condition->next=list_cond;	
	list_cond->prev=cond->condition;
	cond->condition->prev=NULL;
	list_cond=cond->condition;
	return SETERR(OK);
}

int pthread_cond_destroy(pthread_cond_t * cond){
	pthread_initialize();
	if(cond==NULL)/*Non esiste l'elemento passato*/
		return SETERR(EINVAL);
	if(cond->condition==NULL)/*Non è inizializzato oppure è già stato distrutto*/
		return SETERR(EINVAL);
	
	if(cond->condition->list_head!=NULL)/*Qualcuno è in attesa sulla wait*/
		return SETERR(EBUSY);
	SEARCH(list_cond,cond->condition)
	free(cond->condition);
	cond->condition=NULL;
	return SETERR(OK);
}

int pthread_cond_wait(pthread_cond_t * cond , pthread_mutex_t * mutex){
/*Faccio controlli sull'integrità dei dati*/
	if((!cond)||(!mutex))
		return SETERR(EINVAL);

	if((!cond->condition)||(!mutex->mux))
		return SETERR(EINVAL);

	if(mutex->mux->state!=LOCK)/*Non è bloccato il mutex*/
		return SETERR(EINVAL);

	if(mutex->mux->own!=ESECUTION_TID)
		return SETERR(EPERM);


	el_cond_t* new=(el_cond_t*)malloc(sizeof(el_cond_t));
	if(!new)
		return SETERR(ENOMEM);

	new->next=NULL;
	new->own=ESECUTION_TID;
	new->mux=mutex;
	pthread_mutex_unlock(mutex);/*Sblocco il mutex*/
	if(!cond->condition->list_head)
		cond->condition->list_head=new;
	else{
		cond->condition->list_tail->next=new;
		cond->condition->list_tail=new;
	}

	pth_sleep(ESECUTION_TID,WAIT);
	pth_switch(thread_exec->ctx,sched);
	pthread_mutex_lock(mutex);
	return SETERR(OK);	
}

int pthread_cond_signal(pthread_cond_t * cond){

	if(!cond)
		return SETERR(EINVAL);
	if(!cond->condition)
		return SETERR(EINVAL);

	if(cond->condition->list_head)
	{ el_cond_t* sleeping=cond->condition->list_head;
	  cond->condition->list_head=cond->condition->list_head->next;
	  pth_unsleep(sleeping,WAIT);
          free(sleeping);

	}
	return SETERR(OK);
}

int pthread_cond_broadcast(pthread_cond_t* cond){

	if(!cond)
		return SETERR(EINVAL);
	if(!cond->condition)
		return SETERR(EINVAL);
	while(cond->condition->list_head)
	{ el_cond_t* sleeping=cond->condition->list_head;
	  cond->condition->list_head=cond->condition->list_head->next;
	  pth_unsleep(sleeping,WAIT);
          free(sleeping);

	}
	return SETERR(OK);

}

