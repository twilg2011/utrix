#include<errno.h>
#include "pth_struct.h"
#define PTHREAD_MUTEX_INITIALIZER {NULL,1}
#define NOT_INIT 0
#define INIT 1
#define LOCK 2
#define DESTROY 3



/*Struttura del mutex*/
typedef struct mutex{
unsigned char state;
unsigned int val;
pthread_t own;/*Tid di chi possiede il mutex*/
struct mutex* next;
struct mutex* prev;
} pth_mutex_t;


/*Struttura più esterna del mutex, è un semplice contenitore*/
typedef struct pthread_mutex_s{
pth_mutex_t* mux;
int init;
} pthread_mutex_s;

typedef pthread_mutex_s pthread_mutex_t;


typedef struct cond{

struct cond* next;
struct cond* prev;
} pth_cond_t;

typedef struct pthread_cond_s{
pth_cond_t* mux;
int init;
} pthread_cond_s;
typedef pthread_cond_s pthread_cond_t ;

extern int 	 pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t * attr);
extern int 	 pthread_mutex_destroy(pthread_mutex_t* mutex);
extern int	 pthread_mutex_lock(pthread_mutex_t *mutex);
extern int 	 pthread_mutex_unlock(pthread_mutex_t *mutex);




/*Lista dei mutex attivi*/
extern pth_mutex_t* list_mux;



