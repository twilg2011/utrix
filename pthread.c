            /*
             *  pthread.c
              *  Utrix-0.1
             *
             *  Created by Matteo Casenove on 05/11/08.
             *  Copyright 2008 __MyCompanyName__. All rights reserved.
             *
             */

#include "pthread_sched.h"
#include "pth_errno.h"

#define TID_MAIN 0 /* Definisce il tid da assegnare al main */ 

                int pthread_initialized= FALSE; /* Definisce che la libreria non e' stata ancora inizializzata */

                /*
                 * init: Crea il thread che gestira' il main e inizializza lo scheduler come un thread.
                 *
                 * @return FALSE se e' accorso un errore duranete la creazione del thread
                 * @return TRUE se l'inizializzazione e' avvenuta con successo 
                 */
            
    
            
                int init(){
                    
                    tbl_field_t tbl;
                    tcb_t tcb;
                    
                    pth_globalsp_init; /* Inizializzo lo stac alla posizione attuale */
                    
                    /* Definisco la tabella dei thread */
                    tbl=(tbl_field_t)calloc(1,sizeof(tbl_field_s));
                    if(!tbl)
                        return FALSE;
                    
                    /* Inizializza il tcb del main */
                    tcb=(tcb_t)calloc(1,sizeof(tcb_s));
                    if(!tcb)
                        return FALSE;
                    
                    tcb->tid_f=-1;
                    tcb->tid=tcb_n=TID_MAIN;
                    tcb->prior=DEFAULT_PRIOR;
                    tcb->state=NUOVO;

                        tcb->ctx=(context_t)calloc(1,sizeof(context_s));
                            tcb->ctx->eseguito=1;	
                        if(!tcb->ctx){
                            free(tbl);
                            free(tcb);
                            return FALSE;
                        }
                        
                        
                        /* Inizializzo la tabella dei thread */ 
                        tbl->tcb=tcb;
                        tbl->next=NULL;
                        
                        thread_n=1;/* Inizializzo il numero di thread ad 1 avendo inserito il main */
                        
                            
                        thread_new=thread_new_c=tbl; /* Metto nella code dei thread nuovi il main in attesa di essere schedulato */
                        
                        sched=(context_t)calloc(1,sizeof(context_s));
                        /* Inizializzo lo scheduler come un thread */
                        sched_init();
                        /*Fare funzione nello scheduler*/
                        
                        thread_exec=tcb; /* Devinisco come il thread in esecuzione il main */
                        
                        return TRUE;
                        
                        
                    }



                    /*
                     * pthread_create: Crea un nuovo thread. Una volta creato, il thread esegue la funzione passata per 
                     *                 argomento con i parametri passati.
                     * @param	pth conterra' il tid del thread creato
                     * @param	att definisce gli attributi del thread
                     * @param	fun rappresenta la procedura di esecuzione del thread
                     * @param	param rappresenta i parametri della procedura passata
                     *
                     * @return EAGAIN se e' accorso un errore duranete la creazione del thread
                     * @return EINVAL se e' stato specificato un attributo non valido
                     * @return OK se' la creazione ha avuto successo
                     */

                    int pthread_create(pthread_t *pth,/* const pthread_attr_t * att,*/ void *(*fun)(void *) , void * param){
                        tcb_t tcb;
                        tbl_field_t tbl;
                        
                        if( pth == NULL ||/* att != NULL ||*/ fun == NULL || param == NULL )
                           return EAGAIN;
                                
                        /* Controlla se la libreria e' stata inizializata */
                        pthread_initialize();

                        
                        tcb=(tcb_t)calloc(1,sizeof(tcb_s));
                        if(!tcb)
                            return EAGAIN;
                        
                        tbl=(tbl_field_t)calloc(1,sizeof(tbl_field_s));
                        if(!tbl){
                            free(tcb);
                            return EAGAIN;
                        }
                        
                        tcb->tid_f=thread_exec->tid;
                        tcb->tid=(*pth)=++tcb_n;
                        tcb->save=JOINABLE;
                        tcb->prior=DEFAULT_PRIOR;
                        tcb->ctx=(context_t)calloc(1,sizeof(context_s));
                        if(!tcb->ctx){
                            free(tcb);
                            free(tbl);
                            return EAGAIN;
                        }
                        tcb->state=NUOVO;
                        
                        /* Inizializzo il contesto del thread */
                        tcb->ctx->f=fun;
                        tcb->ctx->arg=param;
                        tcb->ctx->eseguito=0;
                        
                        
                        tbl->tcb=tcb;
                        
                        /* Metto in coda dei thread nuovi il thread appena creato */
                        if(thread_new==NULL)
                            thread_new=thread_new_c=tbl;
                        else
                            thread_new_c=thread_new_c->next=tbl;
                        
                        thread_n++;
                        
                        
                        /* Chiamo lo scheduler */
                          pth_switch(thread_exec->ctx,sched);
                        
                        return SETERR(OK);
                        
                    }	




                    /*Prendo il thread attuale cancello tt le informazioni lo metto in stato di morto,tolgo tt,chiamo lo sched*/
                    /*4 Casi:
                     a)join e sono morto, la join prenderà le informazioni necessarie e ripulirà il tutto
                     b)join e non sono morto, si mette in attesa(come fare questo ragionare) creo una lista di join in attesa e cerco dentro se ce ne è una che aspetta il mio tid nella exit(si puo fare)
                     c)detach e sono morto, libero tutto e ignoro il contenuto
                     d)detach e non sono morto devo avvisare di ripulire e ignorare il tutto*/
                    /*Quasi finite da controllare il funzionamento*/





                    /*pthread_self:Questa funzione restituisce il tid del thread in esecuzione(STANDARD)
                     @return: un valore di tipo pthread_t uguale al tid*/

                    pthread_t pthread_self(void){
                        pthread_initialize();
                        return ESECUTION_TID;
                        



                    }

                    void pthread_yield() 
                    {
                     pth_switch(thread_exec->ctx,sched);
                    }






                    /*pthread_join:Questa funzione aspetta la terminazione del thread e restituisce il valore di ritorno del thread in value_ptr(STANDARD)
                     @param: thread è il tid del thread che devo attendere
                     @param value_ptr è la zona di memoria che contiene il valore di ritorno del thread che aspetto.
                     @return: OK in caso di successo altrimenti uno degli errori sottostanti
                     
                     @error:	EINVAL se il thread non è di tipo joinable
                     ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
                     EDEADLK se il tid passato come parametro è uguale al tid del thread chiamante
                     */


  int pthread_join(pthread_t thread, void ** value_ptr){
            pthread_initialize();
            if(thread==ESECUTION_TID)
                return SETERR(EDEADLK);
            tcb_t thread_search;
            thread_search=gettcb(thread);
            #ifdef DEBUG
            printf("Thread trovato%d del tid %p\n",thread,thread_search);

            #endif
            if(!thread_search){
            #ifdef DEBUG
            printf("Thread  Non trovato col tid %d\n",thread);
            #endif
            return SETERR(ESRCH);
            }
            if(thread_search->save==DETACHABLE)
            return SETERR(EINVAL);

            if(thread_search->state==ZOMBIE){
            #ifdef DEBUG
            printf("Thread  Zombie col tid %d\n",thread);
            #endif
            /*Controllo se qualcuno ha gia fatto la join*/
                if(thread_search->thread_join){
                #ifdef DEBUG
                printf("Thread ha qualcuno in join col tid %d\n",thread);
                #endif
                    return SETERR(EINVAL);
                } 
                else{
                /*Nessuno ha mai fatto la join*/
                #ifdef DEBUG
                printf("Thread %d: nessuno ha fatto la join \n",thread_exec->tid);
                #endif
        //printf("CONTROLLA STE JOIN%d\n",*(thread_search->thread_res.ptr_res));			
        *value_ptr=(thread_search->thread_res.res);
          
        //printf("GUARDA QUI%d,%d\n",thread,thread_search->tid);
        //printf("SI CANCELLA %d\n",thread_search->tid);        
        schedthrkill(thread_search->tid);
                    thread_n--;
                    return SETERR(OK);
                }
                

            }
            else{
            /*Non è ancora morto*/
#ifdef DEBUG
                printf("Thread %d non è morto\n",thread);
            #endif
        /*Devo mettere in attesa il thread dopo aver salvato tutte le informazioni necessarie*/
        thread_search->thread_join=thread_exec;
        thread_search->thread_res.ptr_res=value_ptr;
        pth_sleep(ESECUTION_TID,JOIN);
        pth_switch(thread_exec->ctx,sched);		
       // printf("SONO ARRIVATO QUI\n");
        return SETERR(OK);



        }







    }


    /*pthread_detach:Questa funzione rende un thread detach cioè quando terminerà potrà essere deallocato senza salvare nessun valore di ritorno.(STANDARD)
    @param: thread è il tid del thread che devo rendere detach
        
    @return: OK in caso di successo altrimenti uno degli errori sottostanti

    @error:	EINVAL se il thread non è di tipo joinable
        ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
        
    */
    int  pthread_detach(pthread_t thread){
        pthread_initialize();
            tcb_t thread_search;
        
        thread_search=gettcb(thread);
    /*Cerco sulla lista morti*/
        if(!thread_search)
    return SETERR(ESRCH);/*Come da standard*/
        if(thread_search->state==ZOMBIE)/*Se è in stato di zombie allora lo cancello*/
        {	if(thread_search->thread_join)/*Qualcuno ha gia fatto la join*/
                return SETERR(EINVAL);
        #ifdef DEBUG
            printf("Thread %d Zombie x la detach\n",thread);
            #endif
         //   printf("STO UCCIDENDO %d\n",thread_search->tid);
            schedthrkill(thread_search->tid);
            thread_n--;
            return SETERR(OK);
        }
        else{
    /*Non è zombie quindi lo pongo di tipo detach*/
            
            if(thread_search->save!=JOINABLE)/*Non è join*/
            {        
                    return SETERR(EINVAL);
            }	else{
                    thread_search->save=DETACHABLE;
                    return SETERR(OK);
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
        pthread_initialize();
        if(ESECUTION_TID==TID_MAIN)
        {
        /*Ripulisco la libreria*/
        /*Esco*/
            exit((int)((long)value_ptr));
        }
   // printf("Thread%d ha stato%d\n",ESECUTION_TID, thread_exec->save);
        if(thread_exec->save==JOINABLE){
            if(thread_exec->thread_join){
        /*Qualcuno mi aspetta*/
                *(thread_exec->thread_res.ptr_res)=value_ptr;
                #ifdef DEBUG
                printf("Risveglio chi mi aspetta %d\n", thread_exec->thread_join->tid);
                    #endif
     //           printf("NELLA EXIT %d\n",ESECUTION_TID);
           schedthrkill(ESECUTION_TID);
                thread_n--;
                #ifdef DEBUG
                printf("Mi suicido %d e risveglio%d\n",thread_exec->tid,thread_exec->thread_join->tid);
                    #endif/
        pth_unsleep(thread_exec->thread_join->tid,JOIN);
                }
            else{
                #ifdef DEBUG
                printf("Salvo indirizzo %d\n",thread_exec->tid);
                    #endif
       //         printf("OOOOOOO%p\n",thread_exec->thread_res.res);
            (thread_exec->thread_res.res)=value_ptr;
         //   printf("VERIFICHE %d %d",value_ptr,*(thread_exec->thread_res.ptr_res));
            pth_sleep(ESECUTION_TID,ZOMBIESLEEP);
            }
        }
        else{
//printf("UCCIDO NELLA KILL %d\n",ESECUTION_TID);
	schedthrkill(ESECUTION_TID);/*Non è piu presente*/
	thread_n--;
	}
releasepart(thread_exec->ctx->part);
#ifdef DEBUG
			printf("Rilascio la parte %d\n",thread_exec->tid);
         		#endif
pth_switch(thread_exec->ctx,sched);	
}


