/*
 *  pth_context.c
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
#include "pth_context.h"
#include "pth_errno.h"
#include "config.h"
#include <stdlib.h>

partition_t partitionhead;
partition_t partitiontail;
int thread_n;
char* globalsp;

partition_t findfree();

char* bpcalc(context_t ctx)
{
   partition_t part=NULL;
   /*cerco partizioni libere*/
   if (!findfree()) 
   {
     /*se non le trovo ne creo una*/
     part=malloc(sizeof(partition_s));
     if (addpar(part)) return NULL;
   }
   part->present=1;
   ctx->part=part;
   /*ritorno la partizione da usare*/
   return part->bp;
}

/*torna 1 se non esistono partizioni*/ 
int isempty(){
return !partitionhead;
}

/*se è possibile aggiunge una partizione valida*/
int addpar(partition_t new)
{
  /*controla la correttezza dei dati*/
  if (!new || !globalsp) return EINVAL;
    /*se sono presenti altre partizioni*/
	if (isempty()){
	                partitiontail=new;
	                partitionhead=partitiontail;
    }else partitionhead->next=new;
	/*inizializza i dati delle partizioni*/
	new->next=NULL;
	new->bp=globalsp-STACKWIDTH;
	new->present=1;
	globalsp=new->bp;
	return OK;
}

/*cerca una partizione libera*/
partition_t findfree()
{
  partition_t res=partitionhead;
  /*cicla fino a trovare una partizione libera*/
  while (res)
  {
    if(!res->present) return res;
    res=res->next;
  }
  return NULL;
}

/*libera la partizione passata come argomento*/
int relasepart(partition_t part)
{
  if (part) part->present=0;
  else EINVAL;
}
