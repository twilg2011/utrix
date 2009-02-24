
#include "pthread_sched.h"
#include "pth_errno.h"
#define MUTEX_LIST 10
#define COND_LIST 20
#define SEARCHELIM(lista,elem) \
		if(lista){ \
		if(!elem->prev) \
			lista=lista->next; \
		else{ \
		if(!elem->next) \
		elem->prev->next=NULL;\
		else{ \
		elem->prev->next=elem->next;\
		elem->next->prev=elem->prev;\
			}\
		} \
	}
#define DEBUGSY
void stampaL(pth_mutex_t* list_mux){
//printf("Stampo lista mutex\n");
while(list_mux){
printf("Mutex%p\n",list_mux);
list_mux=list_mux->next;


}
printf("Mutex Fine\n");


}



void stampaC(pth_cond_t* list_cond){
printf("Stampo lista condition\n");
while(list_cond){
printf("Condition%p\n",list_cond);
list_cond=list_cond->next;


}
printf("Condition Fine\n");


}

int controllo(void * elem, int lista){
	if(lista==MUTEX_LIST){
		pth_mutex_t* lista=list_mux;
		while(lista && lista!=elem) lista=lista->next;
		return lista?1:0;
		
	}
	else if(lista==COND_LIST){
		
		pth_cond_t* lista=list_cond;
		while(lista && lista!=elem) lista=lista->next;
		return lista?1:0;
		
	}
	return 0;
	
	
	
	
	
	
	
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
int pthread_mutex_init(pthread_mutex_t* mutex,const pthread_mutexattr_t * attr){
	pthread_initialize();

    if(!mutex)
		return SETERR(EINVAL);
	if(attr)
		return SETERR(EINVAL);

    if((mutex->active==ACTIVE) && (mutex->mux)&& (controllo(mutex->mux,MUTEX_LIST)))
        return SETERR(EBUSY);
    int attivoMacro=0;
	printf("GUARDA QUI%d,%d,%p\n",mutex->active,ACTIVE,mutex->mux);
    if(mutex->active==ACTIVE && ((!mutex->mux)||((mutex->mux)&&(!controllo(mutex->mux,MUTEX_LIST)))))
    attivoMacro=1;
    
    
        printf("%d,%p,%d\n",mutex->active,mutex->mux,controllo(mutex->mux,MUTEX_LIST));	

		
	#ifdef DEBUGSY
	printf("Tutti i controlli di integrità nella pthread_mutex_init sono stati superati\n");
	#endif
 	
	mutex->mux=(pth_mutex_t*)calloc(1,sizeof(pth_mutex_t));
	if(!mutex->mux)
		return SETERR(EAGAIN);
	
	
	mutex->mux->state=INIT;
	mutex->mux->val=0;
	mutex->mux->own=0;
	mutex->mux->prev=NULL;
	mutex->mux->next=list_mux;
    mutex->mux->list_head=mutex->mux->list_tail=NULL;
	if(!list_mux)
	list_mux=mutex->mux;
	else{
	list_mux->prev=mutex->mux;
	list_mux=mutex->mux;
	}

	#ifdef DEBUGSY
	printf("Creato e aggiunto alla lista il mutex\n");
	#endif
if(mutex->active!=ACTIVE)
    mutex->active=ACTIVE;

if(attivoMacro)
    return SETERR(EBUSY);
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
	{
		
#ifdef DEBUGSY
		printf("MUtex NULL nella mutex_destroy\n");
#endif
		
		if(mutex->active!=ACTIVE)
			return SETERR(EINVAL);
			
			mutex->active=NO_ACTIVE;
			return SETERR(OK);
		}
	/*Il mutex e' sicuramente diverso da NULL*/
	if(controllo(mutex->mux,MUTEX_LIST)){/*Se non e' vero c'e' qualche problema*/
	if(mutex->mux->state==LOCK)
		return SETERR(EBUSY);
	#ifdef DEBUGSY
	stampaL(list_mux);	
	#endif
	SEARCHELIM(list_mux,mutex->mux)
	#ifdef DEBUGSY
	stampaL(list_mux);	
	#endif
	free(mutex->mux);
	mutex->mux=NULL;
	mutex->active=NO_ACTIVE;
	#ifdef DEBUGSY
	printf("Tutto ok nella mutex_destroy\n");
	#endif
	return SETERR(OK);
	}
	else{
#ifdef DEBUGSY
		printf("Trovato un mutex non creato ma con valore errato nella mutex_destroy\n");
#endif
		
		mutex->mux=NULL;
		if(mutex->active!=ACTIVE)
		return SETERR(EINVAL);
	
		mutex->active=NO_ACTIVE;
		return SETERR(OK);
	}
		
}

/*pthread_mutex_lock:Eseguo una lock su un mutex 
@param: mutex: il mutex da inizializzare
@return:OK se è andato tutto bene, altrimenti un errore
@error:EINVAL se il valore del mutex è sbagliato
       EDEADLK se il proprietario del mutex esegue una lock su un mutex già bloccato da lui.
	ENOMEM */
	int pthread_mutex_lock(pthread_mutex_t *mutex){
	pthread_initialize();
/*Se introduco gli attributi qui qualcosa cambia*/
	if(!mutex)
		return SETERR(EINVAL);
	if(mutex->active!=ACTIVE)
	//	if((!mutex->mux) ||(mutex->mux && !controllo(mutex->mux,MUTEX_LIST)))
			return SETERR(EINVAL);
	
		   int valore=-1;
		if(mutex->active==ACTIVE)
		   if((!mutex->mux)||(mutex->mux && !controllo(mutex->mux,MUTEX_LIST)))/*Se e' attivo ma non c'e' il mutex oppure per errore e' presente un valore io creo il nuovo mutex*/
           {	valore=pthread_mutex_init(mutex,NULL);
               if(valore!=OK && valore!=EBUSY)
		            return valore;
           }/*In questo momento dovrebbe essere creato il mutex*/
#ifdef DEBUGSY
			  printf("Il mutex e' stato creato con valore %p e risultato della ricerca %d\n",mutex->mux,controllo(mutex->mux,MUTEX_LIST));
#endif  
		//lock((void*)mutex->mux->val);/*<---------------------cosa faccio rimango inattivo o passo il controllo allo scheduler?*/
	if((mutex->mux->state==LOCK)&&(mutex->mux->own==ESECUTION_TID))
	return SETERR(EDEADLK);
	#ifdef DEBUGSY
	printf("Tutti i controlli di integrità nella pthread_mutex_lock sono stati superati\n");
	#endif
	while(mutex->mux->state==LOCK){
	
	mutexWait* new=(mutexWait*)malloc(sizeof(mutexWait));
	if(!new)
		return SETERR(ENOMEM);
	new->own=ESECUTION_TID;
	new->next=NULL;
	if(mutex->mux->list_head){
		mutex->mux->list_tail->next=new;
		mutex->mux->list_tail=new;
		}
		else{
		mutex->mux->list_head=mutex->mux->list_tail=new;

		}
	#ifdef DEBUGSY
	printf("Thread %d si addormenta\n",ESECUTION_TID);
	#endif
	pth_sleep(ESECUTION_TID,LOCKSLEEP);
 	pth_switch(thread_exec->ctx,sched);

}

	mutex->mux->state=LOCK;
	mutex->mux->own= ESECUTION_TID;
	#ifdef DEBUGSY
	printf("Thread %d acquista lock\n",ESECUTION_TID);
	#endif
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
	if(mutex->active!=ACTIVE)
		//if((!mutex->mux)||(mutex->mux && !controllo(mutex->mux,MUTEX_LIST)))
			return SETERR(EINVAL);
		   int valore=-1;
	if(mutex->active==ACTIVE)
		   if((!mutex->mux)||(mutex->mux && !controllo(mutex->mux,MUTEX_LIST)))
           {/*Se e' attivo ma non e' ancora stato creato lo creo e ignoro l'errore di EBUSY*/
               valore=pthread_mutex_init(mutex,NULL);
               if(valore!=OK && valore!=EBUSY)
              			  return valore;
           }  
		if(mutex->mux->own!=ESECUTION_TID)
			return SETERR(EPERM);
		//unlock((void*)mutex->mux->val);
	#ifdef DEBUGSY
	printf("Thread in unlock supera i vincoli d'integrità\n");
	#endif
	if(mutex->mux->list_head){
	mutexWait* elim=mutex->mux->list_head;
	mutex->mux->list_head=mutex->mux->list_head->next;
	pth_unsleep(elim->own,LOCKSLEEP);
	#ifdef DEBUGSY
	printf("Thread %d risvegliato\n",elim->own);
	#endif
	free(elim);
	
}
	mutex->mux->state=INIT;
	#ifdef DEBUGSY
	printf("Thread %d ritorna\n",ESECUTION_TID);
	#endif	
	return SETERR(OK);

}





int pthread_cond_init(pthread_cond_t *cond ,const pthread_condattr_t *attr){
	pthread_initialize();
	if(!cond)
		return SETERR(EINVAL);
	if(attr)
		return SETERR(EINVAL);
	
	if((cond->active==ACTIVE) && (cond->condition)&& controllo(cond->condition,COND_LIST))
        return SETERR(EBUSY);
    int attivoMacro=0;
    if(cond->active==ACTIVE && ((!cond->condition)||((cond->condition)&&(!controllo(cond->condition,COND_LIST)))))
		attivoMacro=1;
	
	#ifdef DEBUGSY
	printf("Thread in cond_init supera i vincoli d'integrità\n");
	#endif
	
	
	cond->condition=(pth_cond_t*)calloc(1,sizeof(pth_cond_t));
	if(!cond->condition)
		return SETERR(ENOMEM);
	cond->condition->state=INIT;	
	cond->condition->list_head=cond->condition->list_tail=NULL;
	cond->condition->prev=NULL;
    cond->condition->next=list_cond;
#ifdef DEBUGSY
    stampaC(list_cond);
#endif
    if(!list_cond)
	list_cond=cond->condition;
	else 	{
	list_cond->prev=cond->condition;
	list_cond=cond->condition;


	}
	#ifdef DEBUGSY
	printf("cond init creata senza problemi....\n");
stampaC(list_cond);	
#endif
	if(cond->active==NO_ACTIVE)
    cond->active=ACTIVE;
	
	if(attivoMacro)
		return SETERR(EBUSY);
	return SETERR(OK);
}

int pthread_cond_destroy(pthread_cond_t * cond){
	pthread_initialize();
	if(cond==NULL)/*Non esiste l'elemento passato*/
		return SETERR(EINVAL);
	
	if(cond->condition==NULL)/*Non è inizializzato oppure è già stato distrutto*/
	{
		
#ifdef DEBUGSY
		printf("Condition non esiste in pthread_cond_destroy\n");
#endif
		if(cond->active!=ACTIVE)/*Non e' attivo sollevo errore se no non lo considero*/
		return SETERR(EINVAL);
	   
		cond->active=NO_ACTIVE;   
	   return SETERR(OK);
	   }
	
	/*E' sicuramente diverso da NULL cond->condition*/
	if(controllo(cond->condition,COND_LIST)){
	   
		
#ifdef DEBUGSY
		printf("Condition esiste in pthread_cond_destroy\n");
#endif
	if(cond->condition->list_head!=NULL)/*Qualcuno è in attesa sulla wait*/
		return SETERR(EBUSY);
	#ifdef DEBUGSY
	printf("PrimaDESTROY\n");
	stampaC(list_cond);
	#endif
	SEARCHELIM(list_cond,cond->condition)
	free(cond->condition);
	#ifdef DEBUGSY
	printf("DopoDESTROY\n");
	stampaC(list_cond);
	#endif
	cond->condition=NULL;
	cond->active=NO_ACTIVE;
    return SETERR(OK);
	   }
	else{
		  cond->condition=NULL;
#ifdef DEBUGSY
		printf("Condition ha un valore inconsistente in pthread_cond_destroy\n");
#endif
	if(cond->active!=ACTIVE) /*Non e' attivo sollevo errore se no non lo considero*/
	   return SETERR(EINVAL);

	 
	   cond->active=NO_ACTIVE;   
	   return SETERR(OK);
	   }
}

int pthread_cond_wait(pthread_cond_t * cond , pthread_mutex_t * mutex){
/*Faccio controlli sull'integrità dei dati*/
	if((!cond)||(!mutex))
		return SETERR(EINVAL);

	if((cond->active==NO_ACTIVE)||(mutex->active==NO_ACTIVE))
        return SETERR(EINVAL);
	int valore=-1;
	if((!cond->condition) || ((cond->condition)&&(!controllo(cond->condition,COND_LIST))))
		if((valore=pthread_cond_init(cond,NULL))!=OK)
	   return valore;
	if((!mutex->mux)|| ((mutex->mux)&&(!controllo(mutex->mux,MUTEX_LIST))))
    {	valore=pthread_mutex_init(mutex,NULL);
		if(valore!=OK && valore!=EBUSY)
            return valore;
    }
            /*Ora ho cond inizializzate e mutex inizializzati con valori corretti*/
	if(mutex->mux->state!=LOCK)
		return SETERR(EINVAL);
	if(mutex->mux->own!=ESECUTION_TID)
		return SETERR(EPERM);

/*Ora ho visto se è possibile eseguire la wait*/
	#ifdef DEBUGSY
	printf("Thread in cond_wait supera i vincoli d'integrità\n");
	#endif


	el_cond_t* new=(el_cond_t*)malloc(sizeof(el_cond_t));
	if(!new)
		return SETERR(ENOMEM);
	new->own=ESECUTION_TID;
	new->mux=mutex;
	new->next=NULL;
	if(!cond->condition->list_head)
	cond->condition->list_head=cond->condition->list_tail=new;
	else{
	cond->condition->list_tail->next=new;
	cond->condition->list_tail=new;
	}
	#ifdef DEBUGSY
	printf("Thread in cond_wait sblocca mutex\n");
	#endif
	pthread_mutex_unlock(mutex);
	#ifdef DEBUGSY
	printf("Thread in cond_wait si blocca \n");
	#endif
	pth_sleep(ESECUTION_TID,WAIT);
	pth_switch(thread_exec->ctx,sched);
	pthread_mutex_lock(mutex);
	
	return SETERR(OK);




}

int pthread_cond_signal(pthread_cond_t * cond){

	if(!cond)
		return SETERR(EINVAL);
int valore=-1;
	if(cond->active!=ACTIVE)   {
					   
					   //if(!cond->condition)
						//return SETERR(EINVAL);
					   //if(!controllo(cond->condition,COND_LIST))
						return SETERR(EINVAL);
					   }
	else{
		if((!cond->condition) || ((cond->condition)&&(!controllo(cond->condition,COND_LIST))))
        {
            valore=pthread_cond_init(cond,NULL);
            if(valore!=OK && valore!=EBUSY)
                 return valore;
								 }					 

    }

#ifdef DEBUGSY
	printf("Thread in cond_signal supera i vincoli d'integrità\n");
	#endif
	if(cond->condition->list_head)
	{ el_cond_t* sleeping=cond->condition->list_head;
	  cond->condition->list_head=cond->condition->list_head->next;
	  pth_unsleep(sleeping->own,WAIT);
	#ifdef DEBUGSY
	printf("Thread%d in cond_signal si risveglia\n",sleeping->own);
	#endif
          free(sleeping);

	}
	return SETERR(OK);
}

int pthread_cond_broadcast(pthread_cond_t* cond){

	if(!cond)
	   return SETERR(EINVAL);
	int valore=-1;
	if(cond->active!=ACTIVE)   {
								 
		//if(!cond->condition)
		//	return SETERR(EINVAL);
		//if(!controllo(cond->condition,COND_LIST))
			return SETERR(EINVAL);
		}
	else{
		if((!cond->condition) || ((cond->condition)&&(!controllo(cond->condition,COND_LIST))))
        {	
        valore=pthread_cond_init(cond,NULL);
        if(valore!=OK && valore!=EBUSY)
					return valore;
        	}
    }
								 
	while(cond->condition->list_head)
	{ el_cond_t* sleeping=cond->condition->list_head;
	  cond->condition->list_head=cond->condition->list_head->next;
	  pth_unsleep(sleeping->own,WAIT);
          free(sleeping);

	}
	return SETERR(OK);

}

