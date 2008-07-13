/*
 *  prova.h
 *  
 *
 *  Created by lorenzo galeotti on 10/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
//void prova(char* i);

//#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include "pth_struct.h"
//#include <errno.h>
void getContext(context_t* context);
void setContext(context_t* context);
void swich(context_t* context);
void getIp(context_t* context);

/*typedef struct pth_mctx_st pth_mctx_t;
struct pth_mctx_st {
    ucontext_t uc;
    int restored;
    int error;
};

#define pth_mctx_save(mctx) \
        ( (mctx)->error = errno, \
          (mctx)->restored = 0, \
          getcontext(&(mctx)->uc), \
          (mctx)->restored )
		  
#define pth_mctx_restore(mctx) \
        ( errno = (mctx)->error, \
          (mctx)->restored = 1, \
          (void)setcontext(&(mctx)->uc) )

#define pth_mctx_switch(old,new) \
    swapcontext(&((old)->uc), &((new)->uc));




pth_mctx_t* ppp;
pth_mctx_t* ppp1;
*/
context_t* scegli();
context_t* cambia(context_t* context);

int cazzo;

void* t(context_t* context)
{

printf("asadafdad %p\n",context->ip);
printf("asadafdad %p\n",context->esp);
printf("asadafdad %p\n",context->ebp);
printf("asadafdad %p\n",context->eax);
printf("asadafdad %p\n",context->ebx);
setContext(context);//                  <--------------------------------seg foult
swich(context);
}

void* f(void){
printf("dentro\n");
//setContext(context);
}

int main ()
{
  
  context_t* context=malloc(sizeof(context_t));
  context_t* context_n=malloc(sizeof(context_t));
  int x=0;
  getIp(context);
  getContext(context);
  printf("get %p\n",context->ip);
 /* if(x){
  printf("giusto\n");
  return 1;
  }
  x=1;*/
  // cambia(context);
  //getContext(context);
  getContext(context_n);
  context_n->ip=(char*) t;
  context_n->eax=(char*) context;
  //setContext(context_n);
  printf("fuori %p\n",context->ip);
  printf("fuori %p\n",context->esp);
  printf("fuori %p\n",context->ebp);
  printf("fuori %p\n",context->eax);
  printf("fuori %p\n",context->ebx);
  setContext(context_n);
  swich(context_n);  
  printf("nonnnnnn %p\n", context->ip);
}

context_t* cambia(context_t* context){
int x=0;
getIp(context);
if(x)
return NULL;
getContext(context);
x=1;
/*Scelgo il contesto nuovo e lo inserisco*/
/*context_t* context_new=scegli();

setContext(context_new);

swich(context_new);



}

context_t* scegli(){
context_t* cont;
getContext(cont);
getIp(cont);
return cont;*/
}