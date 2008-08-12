/*
 *  pthread.c
 *  Utrix
 *
 *  Created by Matteo Casenove on 10/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */



#include "pthread_sched.h"


/*
#define pthread_initialize() \

			do{ \
				if (pthread_initialized == FALSE) { \
					pthread_initialized = TRUE; \
					if(!init()) \
						return EAGAIN; \
				} \
			}while(0)

*/


/* Modificare con il tid del processo in esecuzione */

/* #define CREATE_TID(tcb_n,pfun) tcb_t tcb=(tcb_t)malloc(sizeof(tcb_s)); \
								if(!tcb) return FALSE; \ // Controllo che il thread nn puo' essere creato 
								tcb.tid_f=ESECUTION_TID; \ 
								tcb.tid=tcb_n; \
								tcb.prior=DEFAULT_PRIOR; \
								tbl_field_t tblx=(tbl_filed_t)malloc(sizeof(tbl_field_s)); \
								if(!tblx){ \
									free(tcb); \
									return FALSE; \
								} \
								tblx.tcb=tcb; \	
								tblx.next=pth_prior_table[PRIOR(DEFAULT_PRIOR)]; \
								pth_prior_table[PRIOR(DEFAULT_PRIOR)]=tblx;
								******/



//int main(void); /* Dichiaration of main */



/*
 * init: Crea il thread che gestira' il main e inizializza lo scheduler come un thread.
 *
 * @return FALSE se e' accorso un errore duranete la creazione del thread
 * @return TRUE se' la funzione ha avuto successo
 */
int init(){
	pthread_t tid = tcb_n;
	
	tcb_t tcb=(tcb_t)malloc(sizeof(tcb_s));
	if(!tcb) return FALSE;
	tcb->tid_f=NULL;
	tcb->tid=tcb_n;
	tcb->result=NULL;
	tcb->thread_join=NULL;
	
	/* Creo il contesto del main salvandolo */
	pth_save(tcb->ctx)
	
	tcb->state=NUOVO;
	
	tbl_field_t tblx=(tbl_filed_t)malloc(sizeof(tbl_field_s));
	if(!tblx){
		free(tcb);
		return FALSE;
	}
	tblx->tcb=tcb;
	tblx->next=NULL;
	thread_new=tblx;
	thread_n++;
	thread_new=tblx;
	
	/* Inizializzo lo scheduler come un thread */
	pth_init(sched,scheduler,NULL)
	
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
	tbl_field_t tblx;
	tbl_field_t tblfind;
	
	if( pth == NULL || att != NULL || fun == NULL || param == NULL )
		return EAGAIN;
	
	/* Controlla se la libreria e' stata inizializata */
	pthread_initialize();
	
	/* Crea ed inizializza la struttuara del tcb */
	
	tcb=(tcb_t)malloc(sizeof(tcb_s)); 
	if(!tcb) 
		return EAGAIN;  /* Controllo che il thread nn puo' essere creato */
	tcb->tid_f=ESECUTION_TID;  
	tcb->tid=++tcb_n;
	
	tcb->result=NULL;
	tcb->thread_join=NULL;
	/* Creo il contesto del thread */
	pth_init(tcb->ctx,fun,param)
	
	tcb->state=NUOVO;
	tblx=(tbl_filed_t)malloc(sizeof(tbl_field_s)); 
	if(!tblx){ 
		free(tcb); 
		return EAGAIN; 
	} 
	tblx->tcb=tcb;
	
	/* Mette nella coda di schedulazione il thread caricato */
	tblfind=thread_new;
	while(tblfind->next)
		tblfind=tblfind->next;
	tblfind->next=tblx;
	tblx->next=NULL;
	thread_n++;
	/* Chiamo lo scheduler */
	scheduler(NULL);
	
}

