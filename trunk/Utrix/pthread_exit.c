



/*Prendo il thread attuale cancello tt le informazioni lo metto in stato di morto,tolgo tt,chiamo lo sched*/
/*4 Casi:
a)join e sono morto, la join prenderà le informazioni necessarie e ripulirà il tutto
b)join e non sono morto, si mette in attesa(come fare questo ragionare) creo una lista di join in attesa e cerco dentro se ce ne è una che aspetta il mio tid nella exit(si puo fare)
c)detach e sono morto, libero tutto e ignoro il contenuto
d)detach e non sono morto devo avvisare di ripulire e ignorare il tutto*/

void delete(tcb_t thread);/*Ripulisce l'ambiente*/

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
			if(lista->next)/*Sono uscito percjè ho lo stesso tid*/
				lista=lista->next->next;
		}
	free(thread);
}




