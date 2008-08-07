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
/*una partizione consente di identificare un blocco di memoria virtuale 
che rappesenta lo stack del thread di dimensione STACKWIDTH*/
typedef struct partition{
char* bp;/*base pointer*/
unsigned char  present; /*bit di presenza del thread sulla partizione*/
struct partition* next;
} partition_s;

typedef partition_s* partition_t;

/* Thread Context */
typedef struct context{
  jmp_buf regs; /*registri del thread*/
  partition_t part;/*blocco dello stack*/
  void (*f) (void *) ;/*funzione che il thread esegue*/ 
  void * arg;/*argomenti*/
  unsigned char eseguito:1;/*il thread è stato eseguito*/
  } context_s;
  
typedef context_s* context_t;
 /*queste macro permettono di effettuare i cambi di contesto*/

/*pth_save:consente di salvare il contesto attuale 
@param:context_t ctx
*/ 
#define pth_save(ctx) _setjmp(ctx->regs)

/*pth_switch:salva il contesto attuale in old e passa il processore a next
@param:context_t old,context_t next*/
#define pth_switch(old,next) if(_setjmp(old->regs)==0) {_longjmp(next->regs,1);}

/* pth_init:inizializza un contesto che ha come funzione func con argomento argo il contesto inizializzato viene messo in ictx
@param: context_t ictx contesto da inizializzare, void (*f)(void*) funzione del contesto,void* argo argomenti
@error:ERRARG se uno func o ictx sono NULL*/
#define pth_init(ictx,func,argo)\
        if(!ictx||!func) return ERRARG;\
		      ictx->f=func;\
		      ictx->arg=argo;\
                      if (_setjmp(ictx->regs)==1){ \
		          ictx->eseguito=1;\
		          __asm__("movl %0,%%esp"::"r"(bpcalc(ictx)));\
		          func(argo);\
			 }
/*inizializza globalSP allo stack pointer attuale*/
#define pth_globalsp_init __asm__("movl %%ebp,%0":"=r"(globalsp))      
/*bpcalc:calcola uno stackpointer assegnando una partizione al thread utilizza globalSp che deve essere inizializzata	
@param:il contesto a cui assegnare la partizione
@return:il bp della partizione assegnata
*/
char* bpcalc(context_t ctx);
/*relasepart:libera una partizione
  @param:la partizione da liberare
  @return:ERRARG se la partizione è null
*/
int relasepart(partition_t part);

/*sp globale, permette di inizializzare i base pointer dei thread*/
extern char* globalsp;

/*coda di partizioni libere*/
extern partition_t partitionhead;
extern partition_t partiziontail;

