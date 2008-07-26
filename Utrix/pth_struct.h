/*
 *  pth_struct.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 11/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include "pthread.h"
#include "pth_context.h"

/*Qui ci saranno solo le dichiarazioni e strutture necessarie per lavorare con i thread*/

/* Thread Control Block */
typedef struct tcb{
	pthread_t tid_f;
	pthread_t tid;
	void* result;
	partition_t part;
	int prior;
	context_t ctx; 
	char save;
} tcb_s;

typedef tcb_s* tcb_t;

/* Thread table field */
typedef struct tbl_field{
    tcb_t tcb;
    struct tbl_field* next; 
} tbl_field_s;

typedef tbl_field_s* tbl_field_t;

/* Thread table */

//typedef (tbl_field_t *) tcb_table_t;

// tcb_table_t pth_table;

/* Thread table with priority */

extern tbl_field_t pth_prior_table[];
extern int tcb_n;
extern int  thread_n;
extern tcb_t thread_exec;/*Thread attualmente in esecuzione*/
extern tcb_t* thread_zombie;


