/*
 *  pth_errno.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 11/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#define PTH_ERR(func,str,pth_errno) \
#ifdef DEBUG
		fprintf(stderr, \
                 "pth: %s :ERROR: %s\n", func,str); \
#endif
		return(pth_errno);
#define NOERR 0
#define PTHREAD_CREATE 1
#define ERRARG 2