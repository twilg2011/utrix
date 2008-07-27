/*Prendo il thread attuale cancello tt le informazioni lo metto in stato di morto,tolgo tt,chiamo lo sched*/
/*4 Casi:
a)join e sono morto, la join prenderà le informazioni necessarie e ripulirà il tutto
b)join e non sono morto, si mette in attesa(come fare questo ragionare) creo una lista di join in attesa e cerco dentro se ce ne è una che aspetta il mio tid nella exit(si puo fare)
c)detach e sono morto, libero tutto e ignoro il contenuto
d)detach e non sono morto devo avvisare di ripulire e ignorare il tutto*/
#include<errno.h>

#define SEARCH(base,next,ele_cmp,key) while(base) if(base->ele_cmp!=key)base=base->next;

void delete(tcb_t thread);/*Ripulisce l'ambiente*/

pthread_t pthread_self(void){

	return ESECUTION_TID;

}

int pthread_join(pthread_t thread, void ** value_ptr){

if(thread==ESECUTION_TID)
return EDEADLK;

/*Controllo tra quelli morti*/
	tbl_field_t list=thread_zombie;
	int i=0;
	if(SEARCH(list,next,tcb->tid,thread))
	{	/*Lo trovo mi salvo il risultato e lo cancello dal sistema e dalla lista zombie*/
		*value_ptr=list->tcb->result/*Qui da verificare c'è qualcosa che non torna, potrei perderla con il delete*/
		delete(list->tcb);
		deleteZombie(lista->tcb);
		return 0;
	}
	else{/*Se nn lo trovo controllo tra quelli vivi se lo trovo mi metto in coda sulla join*/

		while(i<PRIOR && (list=pth_prior_table[i])){
			SEARCH(list,next,tcb->tid,thread)
			if(!list)
				i++;
			
		}
		if(i==PRIOR)/*Non esiste*/
			return ESRCH;/*Come da standard*/
		else{
			if(!list->tcb->save)/*E' detach*/
				return EINVAL;
			else
			{	if(list->tcb->thread_join)/*Qualcuno ha gia fatto la join*/
				return EINVAL;

				list->tcb->thread_join=thread_exec;/*Salvo il puntatore del thread che mi aspetta*/
			/*Devo bloccare il thread.......*/			
				return 0;
			}
				
				
		}
	}

}

int  pthread_detach(pthread_t thread){
/*Cerco sulla lista morti*/
	tbl_field_t list=thread_zombie;
	
	if(SEARCH(list,next,tcb->tid,thread))
	{       deleteZombie(lista->tcb);
		delete(list->tcb);
		return 0;
	}
	else{
/*Cerco se esiste*/
		list=pth_prior_table[0];
		while(i<PRIOR && (list=pth_prior_table[i])){
			SEARCH(list,next,tcb->tid,thread)
			if(!list)
				i++;
			
		}
		if(i==PRIOR)
			return ESRCH;/*Come da standard*/
		else{
			if(list->tcb->save)/*E join*/
				return EINVAL;
			else
				list->tcb->save=0;
				return 0;
		}
}
/*Se aggiungiamo le opzioni devo fare un controllo in più vedi EINVAL sul manuale*/



}

/*Riguardare*/
void pthread_exit(void* value_ptr){
	if(thread_exec->save){
		tbl_field_t new;
		thread_exec->result=value_ptr;
		thread_exec->part->present=0;/*Non è piu presente sulla partizione,x le specifiche dovrei lasciarlo a 1*/
		new=malloc(sizeof(tbl_field_s));
		new->next=thread_zombie;
		new->tcb=thread_exec;
		thread_zombie=new;
				}
	else{ 
		thread_exec->part->present=0;/*Non è piu presente sulla partizione*/
		delete(thread_exec);
		}
/*chiamo scheduler*/
}


void deleteZombie(tcb_t thread){
	tbl_field_t list=thread_zombie;
	tbl_field_t delete;	
	if(list->tcb->tid==thread->tid)
	{	thread_zombie=thread_zombie->next;
		free(list);
	}	
	else{
	while(list->next && list->next->tcb->tid!=thread->tid)
		list=list->next;
	if(list->next)
	{	delete=list->next;
		list->next=list->next->next;
		free(delete);
	}
}

void delete(tcb_t thread){
	int prio=thread->prior;
	free(thread->part);
	free(thread->ctx);
		if(pth_prior_table[PRIOR(prio)]->tcb->tid==thread->tid){
			pth_prior_table[PRIOR(prio)]=pth_prior_table[PRIOR(prio)]->next;
		}
		else{
			tcb_table_t list=pth_prior_table[PRIOR(prio)];
			while(list->next && list->next->tcb->tid!=thread->tid)
			list=list->next;
			if(lista->next)/*Sono uscito perchè ho lo stesso tid*/
				list->next=list->next->next;
		}
	free(thread);
}




