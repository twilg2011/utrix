/*
 *  pthread.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 10/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

/* Thread Priority */
#define DEFAULT_PRIOR 0
#define HIGHT_PRIOR -1
#define LOW_PRIOR 1
#define PRIOR(p) (p+1)

/* Default attribute */
#define DEFAULT_ATTR NULL

/* Library status */

#define START 1


typedef unsigned int pthread_t;








