/*
 *  pthread_sched.c
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
#include "config.h"
#include "pthread_sched.h"
#include "pth_errno.h"
#include <stdlib.h>
#include <time.h>


tbl_field_t  thread_priortail[NUM_PRIOR];
tbl_field_t  thread_priorhead[NUM_PRIOR];
tbl_field_t  thread_blocked[NUM_WHY];


context_t sched;
int scheduledthr_n;
clock_t pth_time;
/*macro che elimina elem da list parent deve essere il predecessore di elem*/
#define ELIM(elem,parent,list) if (!parent)list=list->next;\
				       else parent->next=elem->next;\
					   elem->next=NULL;\
                                         
/*aggiunge un elemento in coda*/
#define ADDELEM(elem,list,head) if (!list){ list=elem;\
	                                     head=list;\
                                          }	\
				         else{list->next=elem;\
						 list=list->next;\
						 }\
						 elem->next=NULL;
/*aggiunge un elemento in coda*/
#define ADDELEMHEAD(elem,list) if (!list) list=elem;\
						  else{\
						   elem->next=list;\
						   list=elem;\
						   }
						   
#ifdef DEBUG 
/*stampa la lista i-esima trà quelle delle priorità*/
void stampalista(int i)
{ 	
tbl_field_t ap=thread_priorhead[i];
 while (ap) 
 {
	 printf("elemento:%i-",ap->tcb->tid);
	 ap=ap->next;
 }
 printf("*\n");
}
#endif

tbl_field_t selectthr();
void longtermsched();
void recalcprior(tbl_field_t thr);

void empty(void* arg){
while(TRUE)
{
 pth_switch(pth_empty,sched);
}
}
/*cerca nella lista list il thread con il tid passato torna il thread ed il predecessore*/
int searchonlist(int tid, tbl_field_t list, tbl_field_t* serc , tbl_field_t* parent)
{
   /*imposto i parametri per scorrere la lista*/
   (*parent)=NULL;
   (*serc)=list;
   /*scorro la lista*/
   while((*serc))
   {
     /*se lo trovo esco e torno TRUE*/
	 if ((*serc)->tcb->tid==tid) return TRUE;
	 (*parent)=(*tcb);
	 (*serc)=(*serc)->next;
   } 
   /*non ho trovato il tid cercato*/
   return FALSE;
}
/*cerca il tid in tutte le liste dello scheduler*/
int searchonall(int tid,tbl_field_t* serc,tbl_field_t* parent)
{
  int i=0;
  while (i<NUM_PRIOR)
  {
    if (searchonlist(tid,thread_priorhead[i],serc,parent)) return TRUE;
	i++;
  }
  i=0;
  while (i<NUM_WHY)
  {
    if (searchonlist(tid,thread_blocked[i],serc,parent)) return TRUE;
	i++; 
  }
  return FALSE;
}


void scheduler(void* arg)
{
 #ifdef DEBUG
 printf("scheduler\n");
 #endif
 /*thread schedulato*/
 tbl_field_t  selectedthr;
 /*inizializzo a 0 il nomero dei thread schedulabili*/
 scheduledthr_n=0;
 while(TRUE)
 {
 /*controllo che tutti i nuovi thread siano inseriti*/
 if(scheduledthr_n<thread_n)longtermsched();
  if (!(selectedthr=selectthr())){
	  empty(NULL);
  }else{
  
  #ifdef DEBUG
  printf("selected:%i\n",selectedthr->tcb->tid);
  #endif
  /*imposto lo stato corretto*/
   selectedthr->tcb->state=EXEC;
  /*inizializzo la variabile per il calcolo del tempo di cpu utilizzato*/
  pth_time=clock();
 
  #ifdef DEBUG
  printf("parto%p\n",sched);
  #endif
  
  /*passo al thread che ho selezionarto*/
  pth_switch(sched,selectedthr->tcb->ctx);
  
  #ifdef DEBUG
  printf("ritorno %i\n",selectedthr->tcb->tid);
  #endif
  /*calcolo il tempo che ha utilizzato*/
  pth_time=clock()-pth_time;
  /*aggiorno il suo tempo di cpu globale*/
  selectedthr->tcb->time=+pth_time;
  /*se non è stato bloccato ricalcolo la sua priorità*/
  if (selectedthr->tcb->state==EXEC) recalcprior(selectedthr);
  
  #ifdef DEBUG
  printf("altro giro\n");
  #endif
  }
 }
}



tbl_field_t selectthr()
{  
   int i=-1;
   
   #ifdef DEBUG
   printf("selector\n");
   #endif
   /*scorro i thread schedulabili*/
   while(i<NUM_PRIOR-1)
   {
	 
	 #ifdef DEBUG
     printf("lista:%i\n",i);
	 stampalista(PRIOR(i));
	 #endif
     /*il primo della lista più bassa viene selezionato*/
	 if(thread_priorhead[PRIOR(i)]) return thread_priorhead[PRIOR(i)];
     i++;
   }
   return NULL;
}

/*consente di caricare tutti i thread nuovi nello scheduler*/
void longtermsched()
{ 
 #ifdef DEBUG
 printf("longterm\n");
 #endif
 /*campo della tabella fittizio*/
 tbl_field_t null=NULL;
 /*imposto la variabile di scorrimento della lista nuovi*/
 tbl_field_t  new=thread_new;
 /*scorro la lista e carico i thread*/
 while(new && scheduledthr_n<thread_n)
 {
  /*elimino il thread dalla lista nuovi*/
   ELIM(new,null,thread_new);
   /*inizializzo ciò che riguarda lo scheduler*/
   new->tcb->prior=DEFAULT_PRIOR;
   new->tcb->state=PRONTO;
   new->tcb->time=0;
   /*aggiungo l'elemento alla lista di default*/
   ADDELEM(new,thread_priortail[PRIOR(DEFAULT_PRIOR)],thread_priorhead[PRIOR(DEFAULT_PRIOR)]);
   /*incremento il numero di thread schedulati*/
   scheduledthr_n++;
   
   #ifdef DEBUG
   printf("%i charged tid:%i\n",scheduledthr_n,new->tcb->tid);
   stampalista(PRIOR(DEFAULT_PRIOR));
   #endif
   
   new=thread_new;
 }
}

/*setta la priorità di thr a prior*/
void setprior(tbl_field_t thr,int prior)
{
  #ifdef DEBUG
  printf("setprior:");
  #endif
  /*funzione interna evitabile*/
  if(!thr) 
  { 
    SETERR(ERRARG);
    return;
  }
  tbl_field_t tcb;
  tbl_field_t  parent;
  /*cerco l'elemento*/
 if ( searchonlist(thr->tcb->tid,thread_priorhead[PRIOR(thr->tcb->prior)],&tcb,&parent))
 {
  /*se lo trovo lo sposto*/
  ELIM(tcb, parent,thread_priorhead[PRIOR( thr->tcb->prior)]);
  thr->tcb->prior=prior;
  ADDELEM(tcb,thread_priortail[PRIOR(prior)],thread_priorhead[PRIOR(prior)]);
  } else thr->tcb->prior=prior;/*è bloccato calcolo la priorità ma lo lascio stare*/
  
}
/*ricalcola la priorità del thread*/
void recalcprior(tbl_field_t thr)
{
 if(!thr) SETERR(ERRARG);
 if (thr->tcb->time<=BONUSTIME && thr->tcb->prior>-1) setprior(thr,thr->tcb->prior-1);
 if (thr->tcb->time>=MALUSTIME && thr->tcb->prior<1) setprior(thr,thr->tcb->prior+1); 
  thr->tcb->state=PRONTO;
}


void schedthrkill(int tid)
{
 tbl_field_t kill,parent;
 /*cerco il thread da uccidere*/
 if (!searchonall(tid,&kill,&parent)) {
 SETERR(ERRTID);
 return;
 }
 /*lo elimino dallo scheduler*/
 ELIM(kill,parent,thread_priorhead[PRIOR( kill->tcb->prior)]);
 free(kill);
 scheduledthr_n--;
}

void pth_sleep(int tid,int why)
{
tbl_field_t select_tcb,parent;
#ifdef DEBUG
stampalista(PRIOR(1));
#endif
/*controllo i dati*/
if (searchonall(tid,&select_tcb,&parent) && why<NUM_WHY && why>=0) 
{
   #ifdef DEBUG
   printf("sleep:%i\n",select_tcb->tcb->tid);
   #endif
   /*metto il thread nella lista dedicata*/
   ELIM(select_tcb,parent,thread_priorhead[PRIOR(select_tcb->tcb->prior)]);
   ADDELEMHEAD(select_tcb,thread_blocked[why]);
   /*imposto lo stato*/
   select_tcb->tcb->state=BLOCCATO;
   return;
}
SETERR(ERRARG);
}

void pth_unsleep(int tid,int why){
tbl_field_t selected_tcb,parent;
/*controllo i dati*/
if (why<NUM_WHY && why>=0 && searchonlist(tid,thread_blocked[why],&selected_tcb,&parent) ) 
{
   /*rimetto il thread nella lista corrispondente alla sua priorità*/
   ELIM(selected_tcb,parent,thread_blocked[why]);
   ADDELEM(selected_tcb,thread_priortail[PRIOR(selected_tcb->tcb->prior)],thread_priorhead[PRIOR(selected_tcb->tcb->prior)]);
   selected_tcb->tcb->state=PRONTO;
}
SETERR(ERRARG);
}


 
tcb_t gettcb(int tid){
  tcb_t tcb;
  tcb_t parent;
  int i=0;
  tbl_field_t serc;
  tbl_field_t par;
  /*cerco nelle liste di schedulazione*/
  while (i<NUM_PRIOR)
  {
    if (searchonlist(tid,thread_priortail[i],&serc,&par)) return serc->tcb;
	i++;
  }
  /*nelle liste di boloccaggio*/
  i=0;
  while (i<NUM_WHY)
  {
    if (searchonlist(tid,thread_blocked[i],&serc,&par))return serc->tcb;
	i++; 
  }
  return NULL;
}

void jumanji()
{
int i;
tbl_field_t elim;
for(i=0;i<NUM_PRIOR;i++)
{
 while(thread_priortail[i])
 {
   elim=thread_priortail[i];
   thread_priortail[i]=thread_priortail[i]->next;
   free(elim);
 }
}

for(i=0;i<NUM_WHY;i++)
{
  while(thread_blocked[i])
 {
   elim=thread_blocked[i];
   thread_blocked[i]=thread_blocked[i]->next;
   free(elim);
 }
}

free(sched);

}

