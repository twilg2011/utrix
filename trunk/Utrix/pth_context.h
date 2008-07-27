/*
 *  pth_context.h
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

/*Qui ci saranno solo le strutture e le dichiarazioni riguardanti il contesto*/
 #include <setjmp.h>
/*Stack struct*/
//una partizione consente di identificare un blocco di memoria virtuale 
//che rappesenta lo stack del thread

typedef struct partition{
char* bp;
char* sp;
char  present:1;			//bit di presenza del thread sulla partizione
struct partition* next;
} partition_s;

typedef partition_s* partition_t;

/* Thread Context */
typedef struct context{
  jmp_buf regs;				//contesto del thread
  void (*f) (void *) ;		//funzione che il thread esegue 
  void * arg;				//argomenti
  char eseguito:1;			//il thread è stato eseguito
  char ctrlbit:1;			//il thread è di ritorno da una yeld
  } context_s;
  
typedef context_s* context_t;

context_t pth_init(void (*f) (void*),void* arg);

int pth_start(context_t old, context_t next);


int addpar(partition_t new);

int relasepart(partition_t part);
/*sp globale, permette di inizializzare i base pointer dei thread*/
char* globalSp;
extern partition_t partizionitesta;
extern partition_t partizionicoda;
