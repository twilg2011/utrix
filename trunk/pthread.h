/*
 *  pthread.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 10/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

/*Librerie necessarie*/
#include<stddef.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<errno.h>
struct pthread_mutex_s;
struct pthread_cond_s;
typedef unsigned int pthread_t;
typedef struct pthread_mutex_s pthread_mutex_t;
typedef struct pthread_cond_s pthread_cond_t ;
typedef int pthread_mutexattr_t;


extern int       pthread_create(pthread_t *,/* const pthread_attr_t*,*/ void* (*)(void *), void *);
extern int       pthread_detach(pthread_t);
extern void       pthread_yield(void);
extern void      pthread_exit(void *);
extern int       pthread_join(pthread_t, void **);
extern int       pthread_kill(pthread_t, int);
extern pthread_t pthread_self(void);/*Ritorna il tid del thread che la chiama ho pensato di aggiungerla perchè utile che ne dite?*/
extern int 	 pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t * attr);
extern int 	 pthread_mutex_destroy(pthread_mutex_t* mutex);
extern int	 pthread_mutex_lock(pthread_mutex_t *mutex);
extern int 	 pthread_mutex_unlock(pthread_mutex_t *mutex);

//int pthread_attr_init(pthread_attr_t *attr);
//int pthread_attr_destroy(pthread_attr_t *attr);

//int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
//int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);








