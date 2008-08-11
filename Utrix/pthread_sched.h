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

#define NUM_WHY 3
#define MUTEX 0
#define JOIN 1
#define ZOMBIE 2


#define NUOVO 0
#define PRONTO 1
#define BLOCCATO 2
#define EXEC 3

/*contesto dello scheduler*/
extern context_t sched;


/*empty:funzione vuota
@param:NULL
@return:la funzione non ritorna al chiamante
*/
void empty(void* arg);

/*scheduler:funzione di scheduling in 
@param:NULL
@return:la funzione non ritorna al chiamante
*/
void scheduler(void* arg);


/*schedulerkill:elimina il tcb tid dallo scheduler 
@param:tid da eliminare
@return:
@except:ERRTID
*/
void schedthrkill(int tid);

/*sleep:consente di far addormentare un thread
@param:il tid del thread da far addormentare e il motivo 
@except:ERRARG
*/
void pth_sleep(int tid,int why);

/*unsleep:sveglia un threadf
@param:thread da svegliare e il motivo
@return:
@except:ERRARG
*/
void pth_unsleep(int tid,int why);

/*gettcb:funzione che cerca un tcb
@param:il tid da cercare
@return:NULL in caso di errore il tcb altrimenti
*/
tcb_t gettcb(int tid);

/*jumanji:dealloca tutte le strutture necessarie allo scheduler e lo scheduler stesso
@param:
@return:
*/
void jumanji();