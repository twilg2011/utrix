/*
 *  main.c
 *  
 *
 *  Created by lorenzo galeotti on 25/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "pth_errno.h"
#include "pthread_sched.h"


context_t cf;
context_t cw;
context_t mctx;
void f(void* arg);
void w(void* arg);
/*
void init()
{
  getPrisp(&globalSp);
  printf("appena prelevato %p\n",globalSp);
}

*/
void f(void* arg)
{
  printf("f:\n");
 /* pth_init(cw,w,NULL);*/
  pth_switch(cf,cw);
  printf("fine f\n");
  pth_switch(cf,mctx);
}

void w(void* arg)
{
  printf("w:\n");
  pth_switch(cw,mctx);
  printf("fine w\n");
  pth_switch(cw,mctx);

}

      
int main(void)
{
    __asm__("movl %%ebp,%0":"=r"(globalsp));
   printf("1\n");
   cw=malloc(sizeof(context_s));
   cf=malloc(sizeof (context_s));
   mctx=malloc(sizeof(context_s));
   sched=malloc(sizeof(context_s));
   pth_init(cf,f,NULL);
   pth_init(cw,w,NULL);
   pth_init(sched,scheduler,NULL);
  /* printf("2\n");
   pth_switch(mctx,cf);
   printf("Nel main\n");
   printf("switch\n");
   pth_switch(mctx,cw);
   printf("3\n");
   pth_switch(mctx,cf);
   //pth_switch(mctx,cw);
   printf("fine\n");*/
   tbl_field_t m,w,f;
   m=malloc(sizeof(tbl_field_s));
   w=malloc(sizeof(tbl_field_s));
   f=malloc(sizeof(tbl_field_s));
   m->next=w;
   m->tcb->ctx=mctx;
   w->next=f;
   w->tcb->ctx=cw;
   f->next=NULL;
   f->tcb->ctx=cf;
   thread_new=m;
   pth_switch(mctx,sched);
}
