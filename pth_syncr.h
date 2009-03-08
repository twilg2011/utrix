#include "pthread_lib.h"
#define INIT 10
#define LOCK 11
#define NO_ACTIVE 0
#define ACTIVE 1
#define PTHREAD_MUTEX_INITIALIZER {NULL,ACTIVE}
#define PTHREAD_COND_INITIALIZER {NULL,ACTIVE}


/*Struttura necessaria per capire chi sta in attesa*/
typedef struct mutexWait{
	pthread_t own;
	struct mutexWait* next;
} mutexWait;


/*Struttura del mutex vero e proprio*/
typedef struct mutex{
	unsigned char state;
	unsigned int val;/*Valore del mutex*/
	pthread_t own;/*Tid di chi possiede il mutex*/
	mutexWait* list_head;
	mutexWait* list_tail;
	struct mutex* next;
	struct mutex* prev;
} pth_mutex_t;


/*Struttura più esterna del mutex, è un semplice contenitore*/
struct pthread_mutex_s{
	pth_mutex_t* mux;
	int active;
};


typedef struct pthread_mutex_s pthread_mutex_t;

/*Struttura necessaria per memorizzare una lista di condition*/
typedef struct el_cond{
	pthread_mutex_t* mux;
	pthread_t own;
	struct el_cond* next;
} el_cond_t;


/*Struttura effettiva di una condition*/
typedef struct cond{
	unsigned int state;
	el_cond_t* list_head;
	el_cond_t* list_tail;
	struct cond* next;
	struct cond* prev;
} pth_cond_t;


/*Struttura più esterna della condition, è un semplice contenitore*/
struct pthread_cond_s{
	pth_cond_t* condition;
	int active;
};



typedef struct pthread_cond_s pthread_cond_t ;
typedef int pthread_mutexattr_t;
typedef int pthread_condattr_t;
/*Lista dei mutex attivi*/

extern pth_mutex_t* list_mux;
extern pth_cond_t* list_cond;



