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

#define ELIM(elem,parent,list) if (!parent) list=elem->next;\
				       else parent->next=elem->next;\
					   elem->next=NULL;
#define ADDELEM(elem,list,head) if (!list){ list=elem;\
	                                     head=list;\
                                          }	\
				         else list->next=elem;\
                                         elem->next=NULL;
#define ADDELEMHEAD(elem,list) if (!list) list=elem;\
						  else{\
						   elem->next=list;\
						   list=elem;\
						   }
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
  int i=-1;
  while (i<NUM_PRIOR-1)
  {
    if (searchonlist(tid,thread_priortail[PRIOR(i)],tcb,parent)) return 1;
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
 //printf("scheduler\n");
 pth_empty=malloc(sizeof(context_s));
 pth_init(pth_empty,empty,NULL);
 tbl_field_t  selectedthr;
 scheduledthr_n=0;
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
  if (!(selectedthr=selectthr())){
	  pth_switch(sched,pth_empty);
  }else{
  printf("selected:%i\n",selectedthr->tcb->tid);
  pth_time=clock();

  printf("parto%p\n",sched);
  pth_switch(sched,selectedthr->tcb->ctx);
  printf("ritorno %i\n",selectedthr->tcb->tid);

  pth_time=clock()-pth_time;
  selectedthr->tcb->time=pth_time;
  recalcprior(selectedthr);
  printf("altro giro\n");
  }
 }
}


tbl_field_t selectthr()
{  
   int i=-1;
   //printf("selector\n");
   while(i<NUM_PRIOR-1)
   {
     printf("lista:%i\n",i);
     if(thread_priorhead[PRIOR(i)]) return thread_priorhead[PRIOR(i)];
     i++;
   }
   return NULL;
}

void longtermsched()
{
 //printf("longterm\n");
 tbl_field_t null=NULL;
 tbl_field_t  new=thread_new;
 while(new && scheduledthr_n<thread_n)
 {
   ELIM(new,null,thread_new);
   new->tcb->prior=DEFAULT_PRIOR;

   ADDELEM(new,thread_priortail[PRIOR(DEFAULT_PRIOR)],thread_priorhead[PRIOR(DEFAULT_PRIOR)]);

   scheduledthr_n++;
   printf("%i charged tid:%i\n",scheduledthr_n,new->tcb->tid);
   new=thread_new;
 }
}

 void setprior(tbl_field_t thr,int prior)
{
  printf("setprior:");
  if(!thr) 
  { 
    SETERR(ERRARG);
    return;
  }
  tbl_field_t tcb;
  tbl_field_t  parent;
  searchonlist(thr->tcb->tid,thread_priortail[PRIOR( thr->tcb->prior)],&tcb,&parent);
  ELIM(tcb, parent,thread_priortail[PRIOR( thr->tcb->prior)]);
  thr->tcb->prior=prior;

  printf("%i\n",prior);
  ADDELEM(tcb,thread_priortail[PRIOR(prior)],thread_priorhead[PRIOR(prior)]);

}

void recalcprior(tbl_field_t thr)
{
 /*RR*/
/* if(thr->tcb->tic<=tic[PRIOR(thr->tcb->prior)]){
 if(thr->tcb->prior>-1) setprior(thr,thr->tcb->prior-1);
 }
 else if(thr->prior<1) setprior(thr,thr->tcb->prior+1); */
 //printf("recalc\n");
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
 ELIM(kill,parent,thread_priortail[PRIOR( kill->tcb->prior)]);
 scheduledthr_n--;
}

void pth_sleep(int tid,int why)
{
tbl_field_t tcb,parent;
if (searchonall(tid,&tcb,&parent) && why<NUM_WHY && why>=0) 
{
   printf("sleep:%i\n",tcb->tcb->tid);
   ELIM(tcb,parent,thread_priortail[PRIOR( tcb->tcb->prior)]);
   printf("elim:%i\n",thread_priorhead[PRIOR(0)]->tcb->tid);
   ADDELEMHEAD(tcb,thread_blocked[why]);
   return;
}
printf("ciullo\n");
SETERR(ERRARG);
}

void pth_unsleep(int tid,int why){
tbl_field_t tcb,parent;
if (why<NUM_WHY && why>=0 && searchonlist(tid,thread_blocked[why],&tcb,&parent) ) 
{
   ELIM(tcb,parent,thread_blocked[why]);
   ADDELEM(tcb,thread_priortail[PRIOR(tcb->tcb->prior)],thread_priorhead[PRIOR(tcb->tcb->prior)]);
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



