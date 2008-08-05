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


tbl_field_t thread_priortail[NUM_PRIOR];
tbl_field_t  thread_priorhead[NUM_PRIOR];;
context_t sched;

 
int scheduledthr_n;
clock_t time;

void scheduler(void* arg)
{
 tbl_field_t  selectedthr;
 scheduledthr_n=0;
 sched=malloc(sizeof(context_s));
 while(1)
 {
  if(scheduledthr_n<thread_n)longtermsched();
  selectedthr=selectthr();
  time = clock();
  starttime=time;
  pth_swap(sched,selectedthr->tcb->ctx);
  time = clock();
  returntime=time;
  selectedthr->tcb->time=returntime-starttime;  
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
 if(thr->tcb->time<=BENEFITTIME){
 if(thr->tcb->prior>-1) setprior(thr,thr->tcb->prior-1);
 }else {
 if(thr->prior<1) setprior(thr,thr->tcb->prior+1); 
 }
}


void thrkill(int  tid)
{
 tbl_field_t kill;
 SERCH(tid,thr);
 
}
