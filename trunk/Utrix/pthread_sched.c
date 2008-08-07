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
#include <stdlib.h>
#include <time.h>
//#include <clock.h>

tbl_field_t  thread_priortail[NUM_PRIOR];
tbl_field_t  thread_priorhead[NUM_PRIOR];
tbl_field_t  thread_blocked[NUM_WHY];
//int tic[NUM_PRIOR];

context_t sched;
int scheduledthr_n;
clock_t time;

#define ELIM(elem,parent) if (!parent) partent=elem->next;\
						  else parent->next=elem->next;\
                          elem.next=NULL;
#define ADDELEM(elem,list) if (!list) list=elem;\
						  else list->next=elem;\
                          elem.next=NULL;
#define ADDELEMHEAD(elem,list) if (!list) list=elem;\
						  else{\
						   elem->next=list;\
						   list=elem;\
						   }

int serchonlist(int tid, tbl_field_t list, tbl_field_t* tcb , tbl_field_t* parent)
{
   (*parent)=NULL;
   (*tcb)=list;
   while((*tcb))
   {
	 if ((*tcb)->tid==tid) return 1;
	 (*parent)=(*tcb);
	 (*tcb)=(*tcb)->next;
   } 
}

int serchonall(int tid,tbl_field_t* tcb,tbl_field_t* parent)
{
  while (i<NUM_PRIOR)
  {
    if (serchonlist(tid,thread_priortail[PRIOR(i)],tcb,parent)) return 1;
	i++;
  }
  i=0;
  while (i<NUM_WHY)
  {
    if (serchonlist(tid,thread_blocked[i],tcb,parent)) return 1;
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
 tbl_field_t  selectedthr;
 scheduledthr_n=0;
 int i=0;
 while(i<NUM_PRIOR){tic[i]=i; i++;}
 if (!sched=malloc(sizeof(context_s))) {
 SETERR(ENOMEM);
 abort();
 }
 while(1)
 {
 if(scheduledthr_n<thread_n)longtermsched();
 /*RR/*
  /*selectedthr=selectthr();
  selectedthr->tcb->tic=0;
  while(selectedthr->tcb->tic<tic[PRIOR(selectedthr->tcb->prior)])
  {
    //set watchdog
    pth_swap(sched,selectedthr->tcb->ctx);
    selectedthr->tcb->tic++;
  }
  recalcprior(selectedthr);*/
  /*liste*/
  selectedthr=selectthr();
  time=clock();
  pth_swap(sched,selectedthr->tcb->ctx);
  time=clock()-time;
  selectedthr->tcb->time=time;
  recalcprior(selectedthr);
 }
}


intern tbl_field_t selectthr()
{  
   int i=-1;
   while(i<NUM_PRIOR)
   {
     if(thread_priorhead[PRIOR(i)]) return thread_priorhead[PRIOR(i)];
	 i++;
   }
   return NULL;
}

void longtermsched()
{
 tbl_field_t new;
 while(scheduledthr_n<thread_n)
 {
   new=thread_new;
   thread_new=-thread_new->next;
   new->tcb->prior=DEFAULT_PRIOR;
   ADDLIST(new,thread_priortail[PRIOR(DEFAULT_PRIOR)]);
   scheduledthr_n++;
 }
}

 void setprior(tbl_field_t thr,int prior)
{
  if(!thr) 
  { 
    SETERR(err);
    return;
  }
  tbl_field_t tcb,parent;
  serchonlist(thr->tcb->tid,&tcb,&parent);
  ELIM(tcb,parent);
  thr->tcb->prior=prior;
  ADDLIST(tcb,thread_priortail[PRIOR(prior)]);
}

void recalcprior(tbl_field_t thr)
{
 /*RR*/
/* if(thr->tcb->tic<=tic[PRIOR(thr->tcb->prior)]){
 if(thr->tcb->prior>-1) setprior(thr,thr->tcb->prior-1);
 }
 else if(thr->prior<1) setprior(thr,thr->tcb->prior+1); */
 if (thr->tcb->time<=BONUSTIME)if(thr->tcb->prior>-1) setprior(thr,thr->tcb->prior-1);
 if (thr->tcb->time>=MALUSTIME) else if(thr->prior<1) setprior(thr,thr->tcb->prior+1); 
}


void schedthrkill(int tid)
{
 tbl_field_t kill,parent;
 if (!serchonall(tid,&kill,&parent)) {
 SETERR(ERRTID);
 return;
 }
 ELIM(kill,parent);
 scheduledthr_n--;
}

void sleep(int tid,int why){
tcb_t tcb,parent;
if (serchonall(tid,&tcb,&parent) && why<NUM_WHY && why>=0) 
{
   ELIM(tcb,parent);
   ADDLISTHEAD(tcb,thread_blocked[why]);
}
SETERR(ERRARG);
}

void unsleep(int tid,int why){
if (why<NUM_WHY && why>=0 && serchonlist(tid,thread_blocked[why],&tcb,&parent) ) 
{
   ELIM(tcb,parent);
   ADDLIST(tcb,thread_priortail[PRIOR(tcb->tcb->prior)]);
}
SETERR(ERRARG);
}


tcb_t gettcb(int tid){
tcb_t tcb,parent;
int i=0;
  tbl_field_t serc,par;
  while (i<NUM_PRIOR)
  {
    if (serchonlist(tid,thread_priortail[PRIOR(i)],&serc,&par)) return serc->tcb;
	i++;
  }
  i=0;
  while (i<NUM_WHY)
  {
    if (serchonlist(tid,thread_blocked[i],&serc,&par))return serc->tcb;
	i++; 
  }
  return NULL;
}



