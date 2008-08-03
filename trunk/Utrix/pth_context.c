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
   //cerco partizioni libere
   if (!findfree()) 
   {
     //se non le trovo ne creo una
     part=malloc(sizeof(partition_s));
     if (addpar(part)) return NULL;
   }
   //ritorno la partizione da usare
   return part->bp;
}

/*torna 1 se non esistono partizioni*/ 
int isempty(){
return !partizionitesta;
}
/*se è possibile aggiunge una partizione valida*/
int addpar(partition_t new)
{
  //controla la correttezza dei dati
  if (!new || !globalSp) return ERRARG;
  //verifica che sia possibile inserire un'altro thread
  if (thread_n<=MAXTHREAD)
  {
    //se sono presenti altre partizioni
	if (isempty()){
	                partizionicoda=new;
	                partizionitesta=partizionicoda;
    }else partizionicoda->next=new;
	//inizializza i dati delle partizioni
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
  //cicla fino a trovare una partizione libera
  while (res)
  {
    if(!res->present) return res;
    res=res->next;
  }
  return NULL;
}
//libera la partizione passata come argomento
int relasepart(partition_t part)
{
  if (part) part->present=0;
  else ERRARG;
}
