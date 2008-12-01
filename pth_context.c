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
int addpar(partition_t new);

char* bpcalc(context_t ctx)
{
   partition_t part=NULL;
   /*cerco partizioni libere*/
   if (!(part=findfree())) 
   {
     /*se non le trovo ne creo una*/
     #ifdef DEBUG
       printf("nuova partizione\n");
     #endif
     part=malloc(sizeof(partition_s));
     if (addpar(part)) return NULL;
   }
#ifdef DEBUG
   printf("sp partizione:%p\n",part->bp);
#endif
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
                    new->next=NULL;
	                partitionhead=partitiontail;
    }else{
        new->next=partitionhead;
        partitionhead=new;
    }
	/*inizializza i dati delle partizioni*/
	new->bp=globalsp-STACKWIDTH;
	new->present=1;
#ifdef DEBUG
    printf("sp:%p",new->bp);
#endif
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

    #ifdef DEBUG
    printf("cerca ancora:%p\n",res->bp);
    #endif
    if(!res->present)return res;
    res=res->next;
  }
  return NULL;
}

/*libera la partizione passata come argomento*/
int releasepart(partition_t part)
{
  #ifdef DEBUG
  printf("relasepart %p\n",part->bp);
  #endif
  if (part) part->present=0;
  else return SETERR(EINVAL);
  return OK;
}
