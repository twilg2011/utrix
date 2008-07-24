/*
 *  pthread.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 10/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
#include "pth_struct.h"
/* Thread Priority */
#define DEFAULT_PRIOR 0
#define HIGHT_PRIOR -1
#define LOW_PRIOR 1
#define PRIOR(p) (p+1)

/* Defautl attribute */
#define DEFAULT_ATTR NULL

typedef int pthread_t;

extern tcb_n;

extern int       pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
extern int       pthread_detach(pthread_t);
extern int       pthread_yield(void);
extern void      pthread_exit(void *);
extern int       pthread_join(pthread_t, void **);
extern int       pthread_kill(pthread_t, int);

/*sp globale, permette di inizializzare i base pointer dei thread*/
char* globalSp;

partition_t partizionitesta;
partition_t partizionicoda;
int  thread_n;

