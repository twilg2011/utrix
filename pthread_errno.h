/*
 *  pthread_errno.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 11/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include<errno.h>

int pth_errno;

#define PTH_ERR(func,str,pth_errno) \
		fprintf(stderr, \
                 "pth: %s :ERROR: %s\n", func,str); \
		return(pth_errno);

#define SETERR(err) errno=err;
#define OK 0




