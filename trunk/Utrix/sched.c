/*Questo file consentedi effettuare le operazioni della libreria a basso. Permette di caricare o scaricare il contesto di un thread
e di effettuare il cambio di contesto tra un thread e l'altro.*/



/*funzioni implementate
* getContext 
* setContext
*
*                    */


#include "pth_struct.h"
#include <stdlib.h>
#include <stdio.h>
//char* epc;
int getContext(context_t* context){
//__asm__("movl %%,%0": "=r" (epc));
__asm__("movl %%ebp,%0": "=r" (context->ebp));
__asm__("movl %%esp,%0": "=r" (context->esp));
__asm__("movl %%eax,%0": "=r" (context->eax));
__asm__("movl %%ebx,%0": "=r" (context->ebx));
__asm__("movl %%ecx,%0": "=r" (context->ecx));
__asm__("movl %%edx,%0": "=r" (context->edx));
__asm__("movl %%ss,%0": "=r" (context->ss));
__asm__("movl %%edi,%0": "=r" (context->edi));
__asm__("movl %%esi,%0": "=r" (context->esi));
return 0;						
}

int setContext(context_t* context)
{
  __asm__("movl %0,%%ebp": :"r" (context->ebp));
  __asm__("movl %0,%%esp": :"r" (context->esp));
  __asm__("movl %0,%%eax": :"r" (context->eax));
  __asm__("movl %0,%%ebx": :"r" (context->ebx));
  __asm__("movl %0,%%ecx": :"r" (context->ecx));
  __asm__("movl %0,%%edx": :"r" (context->edx)); 
  __asm__("movl %0,%%ss" : :"r" (context->ss));
  __asm__("movl %0,%%edi": :"r" (context->edi));
  __asm__("movl %0,%%esi": :"r" (context->esi));
  return 0;
}







/*funzioni di prova*/
int f(context_t* context)
{
  printf("mondo io esisto");
  setContext(context);
  __asm__("ret");
}

int main()
{
   context_t* context=malloc(sizeof(context_t));
   getContext(context);
   f(context);
   setContext(context);

}
