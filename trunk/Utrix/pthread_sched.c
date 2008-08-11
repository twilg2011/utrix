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
//#include <clock.h>


tbl_field_t  thread_priortail[NUM_PRIOR];
tbl_field_t  thread_priorhead[NUM_PRIOR];
tbl_field_t  thread_blocked[NUM_WHY];
//int tic[NUM_PRIOR];

context_t sched;
int scheduledthr_n;
clock_t pth_time;

#define ELIM(elem,parent,list) if (!parent)list=list->next;\
				       else parent->next=elem->next;\
					   elem->next=NULL;\
                                         

#define ADDELEM(elem,list,head) if (!list){ list=elem;\
	                                     head=list;\
                                          }	\
				         else{list->next=elem;\
						 list=list->next;\
						 }\
						 elem->next=NULL;
						 
#define ADDELEMHEAD(elem,list) if (!list) list=elem;\
						  else{\
						   elem->next=list;\
						   list=elem;\
						   }
						   
#ifdef DEBUG 
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
while(1)
{
 pth_switch(pth_empty,sched);
}
}

int searchonlist(int tid, tbl_field_t list, tbl_field_t* serc , tbl_field_t* parent)
{
   (*parent)=NULL;
   (*serc)=list;
   while((*serc))
   {
	 if ((*serc)->tcb->tid==tid) return 1;
	 (*parent)=(*tcb);
	 (*serc)=(*serc)->next;
   } 
}

int searchonall(int tid,tbl_field_t* serc,tbl_field_t* parent)
{
  int i=0;
  while (i<NUM_PRIOR)
  {
    if (searchonlist(tid,thread_priorhead[i],serc,parent)) return 1;
	i++;
  }
  i=0;
  while (i<NUM_WHY)
  {
    if (searchonlist(tid,thread_blocked[i],serc,parent)) return 1;
	i++; 
  }
  return 0;
}


void scheduler(void* arg)
{
 #ifdef DEBUG
 printf("scheduler\n");
 #endif
 tbl_field_t  selectedthr;
 scheduledthr_n=0;
 while(1)
 {
 if(scheduledthr_n<thread_n)longtermsched();
  if (!(selectedthr=selectthr())){
	  empty(NULL);
  }else{
  
  #ifdef DEBUG
  printf("selected:%i\n",selectedthr->tcb->tid);
  #endif
  
  pth_time=clock();
  selectedthr->tcb->state=EXEC;
  #ifdef DEBUG
  printf("parto%p\n",sched);
  #endif
  
  pth_switch(sched,selectedthr->tcb->ctx);
  
  
  
  #ifdef DEBUG
  printf("ritorno %i\n",selectedthr->tcb->tid);
  #endif
  pth_time=clock()-pth_time;
  selectedthr->tcb->time=+pth_time;
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
   
   while(i<NUM_PRIOR-1)
   {
	 
	 #ifdef DEBUG
     printf("lista:%i\n",i);
	 stampalista(PRIOR(i));
	 #endif
     
	 if(thread_priorhead[PRIOR(i)]) return thread_priorhead[PRIOR(i)];
     i++;
   }
   return NULL;
}

void longtermsched()
{ 
 #ifdef DEBUG
 printf("longterm\n");
 #endif
 
 tbl_field_t null=NULL;
 tbl_field_t  new=thread_new;
 while(new && scheduledthr_n<thread_n)
 {
   ELIM(new,null,thread_new);
   new->tcb->prior=DEFAULT_PRIOR;
   new->tcb->state=PRONTO;
   new->tcb->time=0;
   ADDELEM(new,thread_priortail[PRIOR(DEFAULT_PRIOR)],thread_priorhead[PRIOR(DEFAULT_PRIOR)]);
   scheduledthr_n++;
   
   #ifdef DEBUG
   printf("%i charged tid:%i\n",scheduledthr_n,new->tcb->tid);
   stampalista(PRIOR(DEFAULT_PRIOR));
   #endif

   new=thread_new;
 }
}

 void setprior(tbl_field_t thr,int prior)
{
  #ifdef DEBUG
  printf("setprior:");
  #endif
  
  if(!thr) 
  { 
    SETERR(ERRARG);
    return;
  }
  tbl_field_t tcb;
  tbl_field_t  parent;
 if ( searchonlist(thr->tcb->tid,thread_priorhead[PRIOR(thr->tcb->prior)],&tcb,&parent))
 {
  ELIM(tcb, parent,thread_priorhead[PRIOR( thr->tcb->prior)]);
  thr->tcb->prior=prior;
  ADDELEM(tcb,thread_priortail[PRIOR(prior)],thread_priorhead[PRIOR(prior)]);
  } else thr->tcb->prior=prior;
  
}

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
 if (!searchonall(tid,&kill,&parent)) {
 SETERR(ERRTID);
 return;
 }
 ELIM(kill,parent,thread_priorhead[PRIOR( kill->tcb->prior)]);
 scheduledthr_n--;
}

void pth_sleep(int tid,int why)
{
tbl_field_t select_tcb,parent;
#ifdef DEBUG
stampalista(PRIOR(1));
#endif
if (searchonall(tid,&select_tcb,&parent) && why<NUM_WHY && why>=0) 
{
   #ifdef DEBUG
   printf("sleep:%i\n",select_tcb->tcb->tid);
   #endif
   ELIM(select_tcb,parent,thread_priorhead[PRIOR(select_tcb->tcb->prior)]);
   ADDELEMHEAD(select_tcb,thread_blocked[why]);
   select_tcb->tcb->state=BLOCCATO;
   return;
}
SETERR(ERRARG);
}

void pth_unsleep(int tid,int why){
tbl_field_t selected_tcb,parent;
if (why<NUM_WHY && why>=0 && searchonlist(tid,thread_blocked[why],&selected_tcb,&parent) ) 
{
   ELIM(selected_tcb,parent,thread_blocked[why]);
   ADDELEM(selected_tcb,thread_priortail[PRIOR(selected_tcb->tcb->prior)],thread_priorhead[PRIOR(selected_tcb->tcb->prior)]);
   selected_tcb->tcb->state=PRONTO;
}
SETERR(ERRARG);
}


 
tcb_t gettcb(int tid){
tcb_t tcb;
tcb_t parent;
int i=-1;
  tbl_field_t serc;
  tbl_field_t par;
  while (i<NUM_PRIOR-1)
  {
    if (searchonlist(tid,thread_priortail[PRIOR(i)],&serc,&par)) return serc->tcb;
	i++;
  }
  i=0;
  while (i<NUM_WHY)
  {
    if (searchonlist(tid,thread_blocked[i],&serc,&par))return serc->tcb;
	i++; 
  }
  return NULL;
}



