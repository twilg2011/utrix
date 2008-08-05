/*
 *  pthread_sched.h
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include "pth_struct.h"

/* Thread Priority */
#define NUM_PRIOR 3
#define DEFAULT_PRIOR 0
#define HIGHT_PRIOR -1
#define LOW_PRIOR 1
#define PRIOR(p) (p+1)

#define NUM_WHY 2
#define MUTEX 0
#define JOIN 1
extern context_t sched;

void scheduler(void* arg);

void schedthrkill(int tid);

void sleep(int tid,int why);

void unsleep(int tid,int why);

tcb_t gettcb(int tid);