/*
 *  pthread.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 10/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


// matteo e' un grandissimo gay

typedef int pthread_t;

extern tcb_n;

extern int       pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
extern int       pthread_detach(pthread_t);
extern int       pthread_yield(void);
extern void      pthread_exit(void *);
extern int       pthread_join(pthread_t, void **);
extern int       pthread_kill(pthread_t, int);

/* Thread Priority */
#defne DEFAULT_PRIOR 0
#defne HIGHT_PRIOR -1
#defne LOW_PRIOR 1
#define PRIOR(p) (p+1)

/* Defautl attribute */
#define DEFAULT_ATTR NULL
