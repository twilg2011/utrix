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
#include "pth_struct.h"

tcb_t tcf;
tcb_t tcw;
context_t mctx;


void getPrisp(char** sp);

void init()
{
  getPrisp(&globalSp);
}


void f(void* arg)
{
  printf("f:\n");
  pth_start(tcf->ctx,tcw->ctx);
}

void w(void* arg)
{
  printf("w:\n");
  pth_start(tcw->ctx,mctx);
}



int main(void)
{
   init();
   printf("1\n");
   tcb_s prova;
   prova.ctx=0;
   tcf=malloc(sizeof(tcb_s));
   tcw=malloc(sizeof(tcb_s));
   
   tcf->ctx=pth_init(f,NULL);
   tcw->ctx=pth_init(w,NULL);
   printf("2\n");
   mctx=malloc(sizeof(context_s)); 
   pth_start(mctx,tcf->ctx);
   printf("3:\n");

}
