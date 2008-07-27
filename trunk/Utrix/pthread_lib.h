#include "pthread.h"
/*Qui ci saranno solo le funzioni della libreria*/

extern int       pthread_create(pthread_t *,/* const pthread_attr_t*,*/ void* (*)(void *), void *);
extern int       pthread_detach(pthread_t);
extern int       pthread_yield(void);

extern void      pthread_exit(void *);
extern int       pthread_join(pthread_t, void **);
extern int       pthread_kill(pthread_t, int);
extern pthread_t pthread_self(void);/*Ritorna il tid del thread che la chiama ho pensato di aggiungerla perchè utile che ne dite?*/

