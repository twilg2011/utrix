/*
 *  pth_context.c
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
#include "pth_context.h"
#include "config.h"
#include "pth_errno.h"
#include <stdlib.h>

partition_t partizionitesta;
partition_t partizionicoda;

int thread_n;

partition_t findfree();

char* spcalc()
{
   partition_t part=NULL;
   if (!findfree()) 
   {
     part=malloc(sizeof(partition_s));
     addpar(part);
   }
   return part->bp;
}

int isempty(){
if(!partizionitesta) return 1;
return 0;
}

int addpar(partition_t new)
{
  if (!new || !globalSp) return ERRARG;
  if (thread_n<=MAXTHREAD)
  {
	if (isempty()){
	                partizionicoda=new;
	                partizionitesta=partizionicoda;
    }else partizionicoda->next=new;
	new->next=NULL;
	new->bp=globalSp-STACKWIDTH;
	new->present=1;
	globalSp=new->bp;
	return NOERR;
  }
  return ERRTOOTHR;
}
//cerca una partizione libera
partition_t findfree()
{
  partition_t res=partizionitesta;
  while (res)
  {
    if(!res->present) return res;
    res=res->next;
  }
  return NULL;
}

int relasepart(partition_t part)
{
  if (part) part->present=0;
  else ERRARG;
}
