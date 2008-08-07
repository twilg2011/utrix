#include<errno.h>
#include "pth_struct.h"
#define PTHREAD_MUTEX_INITIALIZER {NULL,1}
#define NOT_INIT 0
#define INIT 1
#define LOCK 2
#define DESTROY 3
#define OK 0


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
struct mutex* mux;
int init;
} pthread_mutex_s;

typedef pthread_mutex_s pthread_mutex_t;

/*Lista dei mutex attivi*/
extern pth_mutex_t* list_mux;



