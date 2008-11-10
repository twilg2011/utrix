/*
 *  pthread.c
 *  Utrix-0.1
 *
 *  Created by Matteo Casenove on 05/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "pthread_sched.h"
#include "pth_errno.h"

#define TID_MAIN 0 /* Definisce il tid da assegnare al main */ 

int pthread_inizialized= FALSE; /* Definisce che la libreria non e' stata ancora inizializzata */

/*
 * init: Crea il thread che gestira' il main e inizializza lo scheduler come un thread.
 *
 * @return FALSE se e' accorso un errore duranete la creazione del thread
 * @return TRUE se l'inizializzazione e' avvenuta con successo 
 */
int init(){
	
	tbl_field_t tbl;
	tcb_t tcb_main;
	
	pth_globalsp_init; /* Inizializzo lo stac alla posizione attuale */
	
	/* Definisco la tabella dei thread */
	tbl=(tbl_field_t)calloc(1,sizeof(tbl_field_s));
	if(!tbl)
		return FALSE;
	
	/* Inizializza il tcb del main */
	tcb_main=(tcb_t)calloc(1,sizeof(tcb_s));
	if(!tcb_main)
		return FALSE;
	
	tcb->tid_f=-1;
	tcb->tid=tcb_n=TID_MAIN;
	tcb->prior=DEFAULT_PRIOR;
	tcb->state=NUOVO;
	tcb->ctx=(context_t)calloc(1,sizeof(context_s));
	if(!tcb->ctx){
		free(tbl);
		free(tcb_main);
		return FALSE;
	}
	
	
	/* Inizializzo la tabella dei thread */ 
	tbl->tcb=tcb;
	tbl->next=NULL;
	
	thread_n=1; /* Inizializzo il numero di thread ad 1 avendo inserito il main */
	
	thread_new=thread_new_c=tbl; /* Metto nella code dei thread nuovi il main in attesa di essere schedulato */
	
	sched=(context_t)calloc(1,sizeof(context_s));
	/* Inizializzo lo scheduler come un thread */
	pth_init(sched,scheduler,NULL)
	pth_init(tcb->context,main,NULL)
	
	thread_exec=tcb; /* Devinisco come il thread in esecuzione il main */
	
	return TRUE;
	
	
}



/*
 * pthread_create: Crea un nuovo thread. Una volta creato, il thread esegue la funzione passata per 
 *                 argomento con i parametri passati.
 * @param	pth conterra' il tid del thread creato
 * @param	att definisce gli attributi del thread
 * @param	fun rappresenta la procedura di esecuzione del thread
 * @param	param rappresenta i parametri della procedura passata
 *
 * @return EAGAIN se e' accorso un errore duranete la creazione del thread
 * @return EINVAL se e' stato specificato un attributo non valido
 * @return OK se' la creazione ha avuto successo
 */

int pthread_create(pthread_t *pth, const pthread_attr_t * att, void *(*fun)(void *) , void * param){
	tcb_t tcb;
	tbl_field_t tbl;
	tbl_field_t ;
	
	tcb=(tcb_t)calloc(1,sizeof(tcb_s));
	if(!tcb)
		return EAGAIN;
	
	tbl=(tbl_field_t)calloc(1,sizeof(tbl_field_s));
	if(!tbl){
		free(tcb);
		return EAGAIN;
	}
	
	tcb->tid_f=thread_exec->tid;
	tcb->tid=++tcb_n;
	tcb->prior=DEFAULT_PRIOR;
	tcb->ctx=(context_t)calloc(1,sizeof(context_s));
	if(!tcb->ctx){
		free(tcb);
		free(tbl);
		return EAGAIN;
	}
	tcb->state=NUOVO;
	
	tbl->tcb=tcb;
	
	/* Metto in coda dei thread nuovi il thread appena creato */
	if(thread_new_c==NULL)
		thread_new=thread_new_c=tbl;
	else
		thread_new_c=thread_new_c->next=tbl;
	
	thread_n++;
	
	/* Chiamo lo scheduler */
	pth_switch(thread_exec->ctx,sched);
	
	return SETERR(OK);
	
}	




/*Prendo il thread attuale cancello tt le informazioni lo metto in stato di morto,tolgo tt,chiamo lo sched*/
/*4 Casi:
 a)join e sono morto, la join prenderà le informazioni necessarie e ripulirà il tutto
 b)join e non sono morto, si mette in attesa(come fare questo ragionare) creo una lista di join in attesa e cerco dentro se ce ne è una che aspetta il mio tid nella exit(si puo fare)
 c)detach e sono morto, libero tutto e ignoro il contenuto
 d)detach e non sono morto devo avvisare di ripulire e ignorare il tutto*/
/*Quasi finite da controllare il funzionamento*/





/*pthread_self:Questa funzione restituisce il tid del thread in esecuzione(STANDARD)
 @return: un valore di tipo pthread_t uguale al tid*/

pthread_t pthread_self(void){
	pthread_initialize();
	return ESECUTION_TID;
	
}

/*pthread_join:Questa funzione aspetta la terminazione del thread e restituisce il valore di ritorno del thread in value_ptr(STANDARD)
 @param: thread è il tid del thread che devo attendere
 @param value_ptr è la zona di memoria che contiene il valore di ritorno del thread che aspetto.
 @return: OK in caso di successo altrimenti uno degli errori sottostanti
 
 @error:	EINVAL se il thread non è di tipo joinable
 ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
 EDEADLK se il tid passato come parametro è uguale al tid del thread chiamante
 */



int pthread_join(pthread_t thread, void ** value_ptr){
	tcb_t thread_search;
	
	pthread_initialize();
	if(thread==ESECUTION_TID)
		return EDEADLK;
	/*Controllo tra quelli morti*/
	thread_search=gettcb(thread);
	
	if(!thread_search)
		return ESRCH;/*Come da standard*/
	
	if(thread_search->state==ZOMBIE)/*Se è in stato di zombie allora faccio le ultime modifiche e poi lo cancello*/
	{	
		if(value_ptr)/*Se non è NULL il valore dove salvare il risultato*/
			*value_ptr=*(thread_search->result);/*Qui da verificare c'è qualcosa che non torna, potrei perderla con il delete*/
		schedthrkill(thread_search->tid);
		thread_n--;
		return OK;
	}
	else{/*Se non è zombie vedo se è joinable o no*/
		
		
		if(thread_search->save==DETACH)/*E' detach*/
			return EINVAL;
		else
		{	if(thread_search->thread_join)/*Qualcuna ha gia fatto la join*/
			return EINVAL;
			
			thread_search->thread_join=thread_exec;/*Salvo il puntatore del thread che mi aspetta*/
			thread_search->result=value_ptr;
			pth_sleep(ESECUTION_TID,JOIN);
			pth_switch(thread_exec->ctx,sched);		
			return OK;
		}
		
		
	}
	
}


/*pthread_detach:Questa funzione rende un thread detach cioè quando terminerà potrà essere deallocato senza salvare nessun valore di ritorno.(STANDARD)
 @param: thread è il tid del thread che devo rendere detach
 
 @return: OK in caso di successo altrimenti uno degli errori sottostanti
 
 @error:	EINVAL se il thread non è di tipo joinable
 ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
 
 */
int  pthread_detach(pthread_t thread){
	
	tcb_t thread_search;
	pthread_initialize();
	thread_search=gettcb(thread);
	/*Cerco sulla lista morti*/
	if(!thread_search)
		return ESRCH;/*Come da standard*/
	if(thread_search->state==ZOMBIE)/*Se è in stato di zombie allora lo cancello*/
	{	
		schedthrkill(thread_search->tid);
		thread_n--;
		return OK;
	}
	else{
		/*Non è zombie quindi lo pongo di tipo detach*/
		
		if(thread_search->save!=JOIN)/*Non è join*/
			return EINVAL;
		else{
			thread_search->save=DETACH;
			return OK;
		}
	}
	/*Se aggiungiamo le opzioni devo fare un controllo in più vedi EINVAL sul manuale*/

}


/*pthread_exit:Questa funzione permette la terminazione del thread e value_ptr sarà il valore di ritorno(STANDARD)
 @param: value_ptr è il valore di ritorno del thread.
 Non dovrebbe mai ritornare
 */


/*Riguardare*/
void pthread_exit(void* value_ptr){
	pthread_initialize();
	if(ESECUTION_TID==TID_MAIN)
	{
		/*Ripulisco la libreria*/
		/*Esco*/
		exit((int)((long)value_ptr));
	}
	releasepart(thread_exec->ctx->part);/*Cancello la partizione*/
	if(thread_exec->save==JOIN){
		if(thread_exec->thread_join){/*Qualcuno aspetta*/
			if(thread_exec->result)/*E non è NULL il posto dove salvare il valore di ritorno*/
				*(thread_exec->result)=value_ptr;/*Nell'indirizzo passato dalla join*/
			pth_unsleep(thread_exec->thread_join->tid,JOIN);
			/*Risveglio thread che ha fatto join*/
			/*Cancello l'attuale thread*/
			schedthrkill(ESECUTION_TID);
			thread_n--;
			
		}
		else{/*Nessuno aspetta*/
			*(thread_exec->result)=value_ptr;
			/*Non è piu presente sulla partizione,x le specifiche dovrei lasciarlo a 1*/
			pth_sleep(ESECUTION_TID,ZOMBIE);
		}
	}
	else{ 
		schedthrkill(ESECUTION_TID);/*Non è piu presente*/
		thread_n--;
		
	}
	pth_switch(thread_exec->ctx,sched);
}

