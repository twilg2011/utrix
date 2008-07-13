/*
 *  pth_struct.h
 *  Utrix
 *
 *  Created by Matteo Casenove on 11/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
 
 #include "pthread.h"
 
/* Thread Context */
typedef struct context{
    char* ip;       /*istuction pointer*/
	char* esp;		/* Stack pointer */
	char* ebp;		/* Stack base pointer */
	char* ss;		/* Segment stack */
	/* General Porpuse Register */
	char* eax;
	char* ebx;
	char* ecx;
	char* edx;
}context_t;

/* Thread Control Block */
typedef struct tcb{
	pthread_t tid;
	void* text;
	int prior;
	context_t* ctx; 
}tcb_t;

/* Thread table field */
typedef struct tbl_field{
    tcb_t tcb;
	struct tbl_field* next; 
}tbl_field_t;

/* Thread table */
tbl_field_t* pth_table;


