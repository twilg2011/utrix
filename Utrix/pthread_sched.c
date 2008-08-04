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

tbl_field_t thread_priortail[NUM_PRIOR];
tbl_field_t thread_priorhead[NUM_PRIOR];
context_t sched;
int scheduledthr_n;


void scheduler(void* arg)
{
 tcb_t selectedthr;
 scheduledthr_n=0;
 sched=malloc(sizeof(context_s));
 while(1)
 {
  if(!selectedthr=selectthr())
  {
	longtermsched();
	selectedthr=selectthr();
  }
  //modifica del tempo
  starttime=getTime();
  pth_swap(sched,selectedthr->ctx);
  returntime=getTime();
  selectedthr->time=returntime-starttime;  
  recalcprior(selectedthr);
 }
}


tcb_t selectthr()
{

}

void longtermsched()
{

}

int timecalc(int oldtime)
{

}

void recalcprior(tcb_t thr)
{

}
