#include "pthread_sched.h"
#define PTHREAD_MUTEX_INITIALIZER {NULL,1}
#define PTHREAD_COND_INITIALIZER {NULL,1}
#define NOT_INIT 0
#define INIT 1
#define LOCK 2
#define DESTROY 3
#define NO_ACTIVE 0
#define ACTIVE 1
#define NO_ACTIVE_COND 0
#define ACTIVE_COND 1

typedef struct mutexWait{
int own;
mutexWait* next;
} mutexWait;


/*Struttura del mutex*/
typedef struct mutex{
unsigned char state;
unsigned int val;
/*pthread_t*/ int own;/*Tid di chi possiede il mutex*/
mutexWait* list_head;
mutexWait* list_tail;
struct mutex* next;
struct mutex* prev;
} pth_mutex_t;


/*Struttura più esterna del mutex, è un semplice contenitore*/
struct pthread_mutex_s{
pth_mutex_t* mux;
int init;
};




typedef struct el_cond{
pthread_mutex_t* mux;
pthread_t own;
struct el_cond* next;
} el_cond_t;

typedef struct cond{
unsigned int state;
el_cond_t* list_head;
el_cond_t* list_tail;
struct cond* next;
struct cond* prev;
} pth_cond_t;

struct pthread_cond_s{
pth_cond_t* condition;
int init;
};




/*Lista dei mutex attivi*/
extern pth_mutex_t* list_mux;
extern pth_cond_t* list_cond;


 
