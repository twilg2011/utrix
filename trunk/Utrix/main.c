/*
 *  main.c
 *  
 *
 *  Created by lorenzo galeotti on 25/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
#include "pth_struct.h"
#include "pth_context.s"
#include "pth_context.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "main.h"

tcb_t tcf;
tcb_t tcw;
context_t mctx;
void getPrisp(char** sp);

void init()
{
  getPrisp(&globalSp);
}


void f(void*)
{
  printf("f:\n");
  pth_start(tcf->ctx,tcw->ctx);
}

void w(void*)
{
  printf("w:\n");
  pth_start(tcw->ctx,mctx);
}



int main(void)
{
   init();
   tcf=malloc(sizeof(tcb_s));
   tcw=malloc(sizeof(tcb_s));
   
   tcf->ctx=pth_init(f,NULL);
   tcw->ctx=pth_init(w,NULL);
   
   mctx=malloc(sizeof(context_s));
    
   pth_start(mctx,tcf->ctx);

}