#include "pthread.h"


/* Default attribute */
#define DEFAULT_ATTR NULL

#define OK 0
#define pthread_initialize() \
			do{ \
				if (pthread_initialized == FALSE) { \
					pthread_initialized = TRUE; \
					init(); \
				} \
			}while(0)



int pthread_initialized = FALSE;
