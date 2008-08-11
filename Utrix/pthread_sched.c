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

context_t sched,pth_empty;
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

int searchonlist(int tid, tbl_field_t list, tbl_field_t* tcb , tbl_field_t* parent)
{
   (*parent)=NULL;
   (*tcb)=list;
   while((*tcb))
   {
	 if ((*tcb)->tcb->tid==tid) return 1;
	 (*parent)=(*tcb);
	 (*tcb)=(*tcb)->next;
   } 
}

int searchonall(int tid,tbl_field_t* tcb,tbl_field_t* parent)
{
  int i=0;
  while (i<NUM_PRIOR)
  {
    if (searchonlist(tid,thread_priorhead[i],tcb,parent)) return 1;
	i++;
  }
  i=0;
  while (i<NUM_WHY)
  {
    if (searchonlist(tid,thread_blocked[i],tcb,parent)) return 1;
	i++; 
  }
  return 0;
}

void rrschedulercaller(void* arg)
{
   context_t ctx=(context_t) arg;
   pth_switch(ctx,sched);
}

void scheduler(void* arg)
{
 #ifdef DEBUG
 printf("scheduler\n");
 #endif
 pth_empty=malloc(sizeof(context_s));
 pth_init(pth_empty,empty,NULL);
 tbl_field_t  selectedthr;
 scheduledthr_n=0;
 while(1)
 {
 if(scheduledthr_n<thread_n)longtermsched();
  if (!(selectedthr=selectthr())){
	  pth_switch(sched,pth_empty);
  }else{
  #ifdef DEBUG
  printf("selected:%i\n",selectedthr->tcb->tid);
  #endif
  
  pth_time=clock();
  
  #ifdef DEBUG
  printf("parto%p\n",sched);
  #endif
  
  pth_switch(sched,selectedthr->tcb->ctx);
  
  #ifdef DEBUG
  printf("ritorno %i\n",selectedthr->tcb->tid);
  #endif
  pth_time=clock()-pth_time;
  selectedthr->tcb->time=pth_time;
  recalcprior(selectedthr);
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
 if (thr->tcb->time<=BONUSTIME && thr->tcb->prior>-1) setprior(thr,thr->tcb->prior-1);
 if (thr->tcb->time>=MALUSTIME && thr->tcb->prior<1) setprior(thr,thr->tcb->prior+1); 
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
tbl_field_t tcb,parent;
#ifdef DEBUG
stampalista(PRIOR(1));
#endif
if (searchonall(tid,&tcb,&parent) && why<NUM_WHY && why>=0) 
{
   #ifdef DEBUG
   printf("sleep:%i\n",tcb->tcb->tid);
   #endif
   ELIM(tcb,parent,thread_priorhead[PRIOR(tcb->tcb->prior)]);
   ADDELEMHEAD(tcb,thread_blocked[why]);
   return;
}
SETERR(ERRARG);
}

void pth_unsleep(int tid,int why){
tbl_field_t tcb,parent;
if (why<NUM_WHY && why>=0 && searchonlist(tid,thread_blocked[why],&tcb,&parent) ) 
{
   ELIM(tcb,parent,thread_blocked[why]);
   ADDELEM(tcb,thread_priortail[PRIOR(tcb->tcb->prior)],thread_priorhead[PRIOR(tcb->tcb->prior)]);
   tcb->tcb->bloccato=0;
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



