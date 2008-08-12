#include "pthread.h"


/* Default attribute */
#define DEFAULT_ATTR NULL

#define FALSE 0
#define TRUE !FALSE
#define pthread_initialize() \
			do{ \
				if (pthread_initialized == FALSE) { \
					pthread_initialized = TRUE; \
					init(); \
				} \
			}while(0)



//int pthread_initialized = FALSE;
