/*
 *  pthread_sched.h
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include "pth_struct.h"

/* Thread Priority */
#define NUM_PRIOR 3
#define DEFAULT_PRIOR 0
#define HIGHT_PRIOR -1
#define LOW_PRIOR 1
#define PRIOR(p) (p+1)

#define NUM_WHY 4
#define MUTEX 0
#define JOIN 1
#define ZOMBIE 2
#define WAIT 3

#define NUOVO 0
#define PRONTO 1
#define BLOCCATO 2
#define EXEC 3

/*contesto dello scheduler*/
extern context_t sched;


/*empty:funzione vuota
 *@param:NULL
 *@return:la funzione non ritorna al chiamante
*/
void empty(void* arg);

/*scheduler:funzione di scheduling in 
 *@param:NULL
 *@return:la funzione non ritorna al chiamante
*/
void scheduler(void* arg);


/*schedulerkill:elimina il tcb tid dallo scheduler 
 *@param:tid da eliminare
 *@except:ESRC se il tid non esiste
*/
void schedthrkill(int tid);

/*sleep:consente di far addormentare un thread
 *@param:tid del thread da far addormentare 
         why il motivo per cui si blocca
 *@except:EINVAL se il why non è tra quelle possibili
        ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
*/
void pth_sleep(int tid,int why);

/*unsleep:sveglia un thread
@param:thread da svegliare e il motivo
@except:EINVAL se il why non è tra quelle possibili
        ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
*/
void pth_unsleep(int tid,int why);

/*gettcb:funzione che cerca un tcb
@param:tid il tid da cercare
@return:NULL in caso di errore il tcb altrimenti
@except:ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
*/
tcb_t gettcb(int tid);

/*jumanji:dealloca tutte le strutture necessarie allo scheduler e lo scheduler stesso
	*/
void jumanji();
