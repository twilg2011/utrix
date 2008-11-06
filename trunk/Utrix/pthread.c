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
 * @return pthread_t il tid del main 
 */
int init(){
	
	tbl_field_t tbl;
	tcb_t tcb_main;
	
	pth_globalsp_init; /* Inizializzo lo stac alla posizione attuale */
	
	/* Definisco la tabella dei thread */
	tbl=(tbl_field_t)malloc(sizeof(tbl_field_s));
	if(!tbl)
		return FALSE;
	
	/* Inizializza il tcb del main */
	tcb_main=(tcb_t)malloc(sizeof(tcb_s));
	if(!tcb_main)
		return FALSE;
	
	tcb->tid_f=-1;
	tcb->tid=TID_MAIN;
	tcb->result=NULL;
	tcb->prior=DEFAULT_PRIOR;
	tcb->state=NUOVO;
	tcb->ctx=(context_t)malloc(sizeof(context_s));
	if(!tcb->ctx){
		free(tbl);
		free(tcb_main);
		return FALSE;
	}
	
	/* Creo il contesto del main salvandolo */
	pth_save(tcb->ctx);
	
	/* Inizializzo la tabella dei thread */ 
	tbl->tcb=tcb;
	tbl->next=NULL;
	
	thread_n=1; /* Inizializzo il numero di thread ad 1 avendo inserito il main */
	
	thread_new=tbl; /* Metto nella code dei thread nuovi il main in attesa di essere schedulato */
	
	sched=(context_t)malloc(sizeof(context_s));
	
	/* Inizializzo lo scheduler come un thread */
	pth_init(sched,scheduler,NULL)
	
	sched=(context_t)malloc(sizeof(context_s));
	/* Inizializzo lo scheduler come un thread */
	pth_init(sched,scheduler,NULL)
	pth_init(tcb->context,main,NULL)
	
	return tcb->tid;
	
	
}
