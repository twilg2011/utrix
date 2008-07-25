/*
 *  pth_context.c
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
#include "pth_stack.s"
#include "pth_context.h"
#include "config.h"
#include "setjmp.h"
#include <stdlib.h>


void setsp(char* sp);
void setbp(char* bp);

context_t pth_init(void (*f) (void*),void* arg)
{
  context_t context=malloc(sizeof(context_s));
  if (!context) return NULL;
  context->eseguito=0;
  context->f=(*f);
  context->arg=arg;
  return context;
}

int pth_start(context_t old, context_t next)
{
  old->ctrlbit=0;
  setjmp(old->regs);
  if (old->ctrlbit) return NOERR;
  if (!(next->eseguito) && !(old->ctrlbit))
  {
    char* sp,bp;
	old->ctrlbit=1;
    next->eseguito=1;
	spcalc(&sp,&bp);
	setsp(sp);
	setbp(bp);
	(next->f)(next->arg);
	//non dovrebbe esegurla mai solo in caso di ret
	return NOERR;
  }
  old->ctrlbit=1;
  longjmp(next->regs);
}

int spclac(char** sp, char** bp)
{
   if(!sp || !bp) return ERRARG;
   partition_t part=NULL;
   if (!findfree()) 
   {
     part=malloc(sizeof(partition_s));
	 addpar(part);
   }
   (*sp)=part->sp;
   (*bp)=part->bp;
   return NOERR;
}

int isempty(){
if(!partition) return 1;
return 0;
}

int addpar(partition_t new)
{
  if (!new) return ERRARG;
  if (nthread<=MAXTHREAD)
  {
	if (isempty()){
	                partizionicoda=new;
	                partizionitesta=partizionicoda;
    }else partizionicoda.next=new;
	new->next=NULL;
	new->bp=globalSp;
	new->sp=globalSp-STACKWIDTH;
	new->present=1;
	globalSp=new->sp;
	return NOERR;
  }
  return ERRTOOTHR;
}

partition_t findfree()
{
  partition_t res=partizionitesta;
  while (res)
  {
    if(!res.present) return res;
	res=res->next;
  }
  return NULL;
}

int relasepart(partition_t part)
{
  if (part) part->present=0;
  else ERRARG;
}
