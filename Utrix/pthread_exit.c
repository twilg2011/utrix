/*Prendo il thread attuale cancello tt le informazioni lo metto in stato di morto,tolgo tt,chiamo lo sched*/
/*4 Casi:
a)join e sono morto, la join prenderà le informazioni necessarie e ripulirà il tutto
b)join e non sono morto, si mette in attesa(come fare questo ragionare) creo una lista di join in attesa e cerco dentro se ce ne è una che aspetta il mio tid nella exit(si puo fare)
c)detach e sono morto, libero tutto e ignoro il contenuto
d)detach e non sono morto devo avvisare di ripulire e ignorare il tutto*/
/*Quasi finite da controllare il funzionamento*/


#include"pth_struct.h"
#define SEARCH(base,next,ele_cmp,key) while(base) if(base->ele_cmp!=key)base=base->next;
#define TID_MAIN 0
void delete(tcb_t thread);/*Ripulisce l'ambiente*/
void deleteZombie(tcb_t thread);

/*pthread_self:Questa funzione restituisce il tid del thread in esecuzione(STANDARD)
@return: un valore di tipo pthread_t uguale al tid*/

pthread_t pthread_self(void){

	return ESECUTION_TID;

}

/*pthread_join:Questa funzione aspetta la terminazione del thread e restituisce il valore di ritorno del thread in value_ptr(STANDARD)
@param: thread è il tid del thread che devo attendere
	value_ptr è la zona di memoria che contiene il valore di ritorno del thread che aspetto.
@return: OK in caso di successo altrimenti uno degli errori sottostanti

@error:	EINVAL se il thread non è di tipo joinable
	ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
	EDEADLK se il tid passato come parametro è uguale al tid del thread chiamante
*/



int pthread_join(pthread_t thread, void ** value_ptr){

if(thread==ESECUTION_TID)
return EDEADLK;

/*Controllo tra quelli morti*/
	tbl_field_t list=thread_zombie;
	int i=0;
	SEARCH(list,next,tcb->tid,thread)
	if(list)
	{	/*Lo trovo mi salvo il risultato e lo cancello dal sistema e dalla lista zombie*/
		*value_ptr=*(list->tcb->result);/*Qui da verificare c'è qualcosa che non torna, potrei perderla con il delete*/
		schedthrkill(list->tcb->tid)
		deleteZombie(list->tcb);
		return 0;
	}
	else{/*Se nn lo trovo controllo tra quelli vivi se lo trovo mi metto in coda sulla join*/

		tcb_t thread_tcb=gettcb(thread)
		if(!thread_tcb)
			return ESRCH;/*Come da standard*/
		else{
			if(thread_tcb->save==DETACH)/*E' detach*/
				return EINVAL;
			else
			{	if(thread_tcb->thread_join)/*Qualcuno ha gia fatto la join*/
				return EINVAL;

				thread_tcb->thread_join=thread_exec;/*Salvo il puntatore del thread che mi aspetta*/
				thread_tcb->result=value_ptr;
				sleep(ESECUTION->TID,JOIN);		
				return OK;
			}
				
				
		}
	}

}


/*pthread_detach:Questa funzione rende un thread detach cioè quando terminerà potrà essere deallocato senza salvare nessun valore di ritorno.(STANDARD)
@param: thread è il tid del thread che devo rendere detach
	
@return: OK in caso di successo altrimenti uno degli errori sottostanti

@error:	EINVAL se il thread non è di tipo joinable
	ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
	
*/
int  pthread_detach(pthread_t thread){
/*Cerco sulla lista morti*/
	tbl_field_t list=thread_zombie;
	int i=0;
	SEARCH(list,next,tcb->tid,thread)
	if(list)
	{     	schedthrkill(list->tcb->tid)
		return OK;
	}
	else{
/*Cerco se esiste*/
		tcb_t thread_tcb=gettcb(thread)
		if(!thread_tcb)
			return ESRCH;/*Come da standard*/
		else{
			if(thread_tcb->save==JOIN)/*E join*/
				return EINVAL;
			else
				thread_tcb->save=DETACH;
				return OK;
		}
}
/*Se aggiungiamo le opzioni devo fare un controllo in più vedi EINVAL sul manuale*/



}


/*pthread_exit:Questa funzione permette la terminazione del thread e value_ptr sarà il valore di ritorno(STANDARD)
@param: value_ptr è il valore di ritorno del thread.
Non dovrebbe mai ritornare
*/


/*Riguardare*/
void pthread_exit(void* value_ptr){
if(ESECUTION_TID==TID_MAIN)
{
/*Ripulisco la libreria*/
/*Esco*/
exit((int)((long)value_ptr));
}
	if(thread_exec->save==JOIN){
		if(thread_exec->thread_join){/*Qualcuno aspetta*/
			*(thread_exec->result)=value_ptr;/*Nell'indirizzo passato dalla join*/
			 thread_exec->part->present=0;
		         unsleep(thread_join->tid,JOIN);
/*Risveglio thread che ha fatto join*/
/*Cancello l'attuale thread*/
			schedthrkill(ESECUTION_TID);
				

			}
		else{/*Nessuno aspetta*/
			*(thread_exec->result)=value_ptr;
			thread_exec->part->present=0;/*Non è piu presente sulla partizione,x le specifiche dovrei lasciarlo a 1*/
			sleep(ESECUTION_TID,ZOMBIE);
				}
	}
	else{ 
		schedthrkill(ESECUTION_TID);/*Non è piu presente*/
		
		}
pth_switch(thread_exec->tcb,sched);
}







