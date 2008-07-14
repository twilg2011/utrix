/*
 *  pthread.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 10/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


typedef int pthread_t;

extern int       pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
extern int       pthread_detach(pthread_t);
extern pthread_t pthread_self(void);
extern int       pthread_equal(pthread_t, pthread_t);
extern int       pthread_yield_np(void);
extern void      pthread_exit(void *);
extern int       pthread_join(pthread_t, void **);
extern int       pthread_once(pthread_once_t *, void (*)(void));
extern int       pthread_sigmask(int, const sigset_t *, sigset_t *);
extern int       pthread_kill(pthread_t, int);