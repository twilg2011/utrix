/*
 *  pth_errno.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 11/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include<errno.h>
int pth_errno;
#define PTH_ERR(func,str,pth_errno) \
		fprintf(stderr, \
                 "pth: %s :ERROR: %s\n", func,str); \
		return(pth_errno);

#define SETERR(err) errno=err;
#define NOERR 0
#define PTHREAD_CREATE 1
#define ERRARG 2
#define ERRTOOTHR 3

/* Errore that can be return in a pthread_create */

#define EAGAIN 3406 /* The system lacked the necessary resources to create
						another thread, or the system-imposed limit  on the
						total number of threads in a process
						[PTHREAD_THREADS_MAX] would be exceeded. */

#define EINVAL 3021     /*  The value specified by attr is invalid. */

