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
tcb_t thread_exec=NULL;
tbl_field_t thread_zombie=NULL;
list_field_t join_wait=NULL;
tbl_field_t pth_prior_table[3];
tbl_field_t pth_last_table_field[3];
