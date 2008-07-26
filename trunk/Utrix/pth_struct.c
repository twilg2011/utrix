/*
 *  pth_struct.c
 *  Utrix
 *
 *  Created by Matteo Casenove on 14/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "pth_struct.h"
int thread_n;
int tcb_n = 0; /* Progressive tcb number. 
				* The tcb_n 0 is assigned to main */
int thread_exec;
tcb_t* thread_zombie;
tbl_field_t pth_prior_table[3];
