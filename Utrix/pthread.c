/*
 *  pthread.c
 *  Utrix
 *
 *  Created by Matteo Casenove on 10/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "pthread.h"
#include "pth_struct.h"
#include  "pth_stack.s"


static int pthread_initialized = FALSE;

#define pthread_initialize() \
			do{ \
				if (pthread_initialized == FALSE) { \
					pthread_initialized = TRUE; \
					init(); \
				} \
			}while(0)


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

/*funzione assembler che permette di individuare lo sp della funzione precedente*/
void getPrisp(char** sp);

//int main(void); /* Dichiaration of main */



int init(){
	pthread_t tid = tcb_n;
	tcb_t tcb=(tcb_t)malloc(sizeof(tcb_s));
	if(!tcb) return FALSE;
	tcb.tid_f=NULL;
	tcb.tid=tcb_n;
	tcb.prior=DEFAULT_PRIOR;
	tbl_field_t tblx=(tbl_filed_t)malloc(sizeof(tbl_field_s));
	if(!tblx){
		free(tcb);
		return FALSE;
	}
	tblx.tcb=tcb;
	tblx.next=pth_prior_table[PRIOR(DEFAULT_PRIOR)]=NULL;
	pth_last_table_field[PRIOR(DEFAULT_PRIOR)]=pth_prior_table[PRIOR(DEFAULT_PRIOR)]=tblx;
}


int pthread_create(pthread_t *pth, /*const pthread_attr_t * att,*/ void *(*fun)(void *) , void * param){
    
	tcb_t tcb;
	tbl_field_t tblx;
	tbl_field_t tblfind;
	
	
	
	if( pth == NULL || /*att != NULL ||*/ fun == NULL || param == NULL )
		return FALSE;
	
	/* Controlla se la libreria e' stata inizializata */
	pthread_initialize();
	
	/* Crea ed inizializza la struttuara del tcb */
	
	// CONTEXT CREATE  
	
	pth=++tcb_n;
	tcb=(tcb_t)malloc(sizeof(tcb_s)); 
	if(!tcb) 
		return FALSE;  /* Controllo che il thread nn puo' essere creato */
	tcb->tid_f=ESECUTION_TID;  
	tcb->tid=tcb_n; 
	tcb->prior=DEFAULT_PRIOR; 
	tblx=(tbl_filed_t)malloc(sizeof(tbl_field_s)); 
	if(!tblx){ 
		free(tcb); 
		return FALSE; 
	} 
	tblx->tcb=tcb;
	
	/* Mette nella coda di schedulazione il thread caricato */
	
	pth_last_table_field[PRIOR(DEFAULT_PRIOR)]->next=tblx;
	tblx->next=NULL;
	pth_last_table_field[PRIOR(DEFAULT_PRIOR)]=tblx; 
	
	//-------->> Controllo dello scheduler
	
}







