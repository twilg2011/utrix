/*
 *  pth_struct.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 11/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
 #include <sys/jmp_buf.h>
 #include "pthread.h"
 
typedef struct partition{
char* sp;
char  present:1;
partition_t next;
}partition_s;
typedef (partition_s*) partition_t;

/* Thread Context */
struct context{
  jmp_buf regs; //contesto del thread
  void (*f) (void *) ;//funzione che il thread esegue 
  void * arg;//argomenti
  char eseguito:1;//il thread è stato eseguito
  char ctrlbit:1;//il thread è di ritorno da una yeld
  }
typedef (struct context) context_t;

/* Thread Control Block */
typedef struct tcb{
	pthread_t tid_f;
	pthread_t tid;
	void* text;
	int prior;
	context_t ctx; 
}tcb_s;

typedef (tcb_s *) tcb_t;

/* Thread table field */
typedef struct tbl_field{
    tcb_t tcb;
	struct tbl_field_t next; 
}tbl_field_s;

typedef (tbl_field_s *) tbl_field_t;

/* Thread table */

//typedef (tbl_field_t *) tcb_table_t;

// tcb_table_t pth_table;

/* Thread table with priority */

extern tbl_field_t pth_prior_table[];

partition_t partizioni;