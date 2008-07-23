/*
 *  pth_context.c
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include "pth_context.h"
#include "setjmp.h"
#include <stdlib.h>


void setsp(char* sp);
void setbp(char* bp);

context_t* pth_init(void (*f) (void*),void* arg)
{
  context_t* context=malloc(sizeof(context_t));
  if (!context) return NULL;
  context->eseguito=0;
  context->f=(*f);
  context->arg=arg;
  return context;
}

int pth_start(context_t* old, context_t* next)
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
	return NOERR;
  }
  old->ctrlbit=1;
  longjmp(next->regs);
}

spclac(char* sp, char* bp)
{

}
