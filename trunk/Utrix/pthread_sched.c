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
#include <stdlib>
#include <time.h>
#include <clock.h>

tbl_field_t  thread_priortail[NUM_PRIOR];
tbl_field_t  thread_priorhead[NUM_PRIOR];
tbl_field_t  thread_blocked[NUM_WHY];
int tic[NUM_PRIOR];
context_t sched;

 
int scheduledthr_n;
clock_t time;

int serchonlist(int tid, tbl_field_t list,tcb_t* tcb,tcb_t* parent)
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


int serch(int tid, tcb_t* tcb,tcb_t* parent)
{ int i=0;
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

void scheduler(void* arg)
{
 tbl_field_t  selectedthr;
 scheduledthr_n=0;
 int i=0;
 while(i<NUM_PRIOR){tic[i]=i; i++;}
 sched=malloc(sizeof(context_s));
 while(1)
 {
  if(scheduledthr_n<thread_n)longtermsched();
  selectedthr=selectthr();
  selectedthr->tcb->tic=0;
  while(selectedthr->tcb->tic<tic[PRIOR(selectedthr->tcb->prior)])
  {
    //set watchdog
    pth_swap(sched,selectedthr->tcb->ctx);
    selectedthr->tcb->tic++;
  }
  recalcprior(selectedthr);
 }
}


tbl_field_t selectthr()
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
   thread_priortail[PRIOR(DEFAULT_PRIOR)]->next=new;
   new->next=NULL;
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
  thr->tcb->prior=prior;
}

void recalcprior(tbl_field_t thr)
{
 if(thr->tcb->tic<=tic[PRIOR(thr->tcb->prior)]){
 if(thr->tcb->prior>-1) setprior(thr,thr->tcb->prior-1);
 }
 else if(thr->prior<1) setprior(thr,thr->tcb->prior+1); 
}


void schedthrkill(int tid)
{
 tbl_field_t kill,parent;
 if (!serch(tid,&kill,&parent)) {
 SETERR(ERRTID);
 return;
 }
 parent->next=kill->next;
 scheduledthr_n--;
}

void sleep(int tid,int why){
}

void unsleep(int tid,int why){
}


tcb_t gettcb(int tid){
tbl_field_t tcb,parent;
if (!serch(tid,&tcb,&parent)){
SETERR(ERRTID);
return NULL;
}
return tbc;
}



