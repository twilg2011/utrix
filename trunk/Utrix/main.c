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

context_t cf;
context_t cw;
context_t mctx;


void getPrisp(char** sp);

void init()
{
  getPrisp(&globalSp);
  printf("appena prelevato %p\n",globalSp);
}


void f(void* arg)
{
  printf("f:\n");
  pth_switch(cf,mctx);
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
   __asm__("movl %%ebp,%0":"=r"(globalSp));
   printf("1\n");
   cw=malloc(sizeof(context_s));
   cf=malloc(sizeof (context_s));
   mctx=malloc(sizeof(context_s));
    pth_init(cf,f,NULL);
   printf("2\n");
   pth_init(cw,w,NULL);
   printf("switch\n");
   pth_switch(mctx,cw);
   printf("3\n");
   pth_switch(mctx,cf);
   pth_switch(mctx,cw);
   printf("fine\n");
}
