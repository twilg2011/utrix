/*
 *  pth_struct.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 11/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
 #include <sys/jmp_buf.h>
 
typedef int pthread_t;
/*Stack struct*/
//una partizione consente di identificare un blocco di memoria virtuale 
//che rappesenta lo stack del thread
typedef struct partition{
char* bp;
char* sp;
char  present:1; //bit di presenza del thread sulla partizione
struct partition* next;
}partition_s;
typedef partition_s* partition_t;

/* Thread Context */
typedef struct context{
  jmp_buf regs; //contesto del thread
  void (*f) (void *) ;//funzione che il thread esegue 
  void * arg;//argomenti
  char eseguito:1;//il thread è stato eseguito
  char ctrlbit:1;//il thread è di ritorno da una yeld
  }context_s;
  
typedef context_s* context_t;

/* Thread Control Block */
typedef struct tcb{
	pthread_t tid_f;
	pthread_t tid;
	void* result;
	partition_t part;
	int prior;
	context_t ctx;
	char save:1; 
}tcb_s;

typedef tcb_s * tcb_t;

/* Thread table field */
typedef struct tbl_field{
    tcb_t tcb;
 struct tbl_field next; 
}tbl_field_s;

typedef tbl_field_s * tbl_field_t;

/* Thread table */

//typedef (tbl_field_t *) tcb_table_t;

// tcb_table_t pth_table;

/* Thread table with priority */
//numero 5 fittizio
extern tbl_field_t pth_prior_table[5];

