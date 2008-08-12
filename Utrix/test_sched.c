/*
 *  test_sched.c
 *  Utrix-0.1
 *
 *  Created by Matteo Casenove on 12/08/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include "pthread.h"


void p1(void * arg){
	int num;
	num=(int)arg;
	printf("Ora e' in funzione il thread %d...\n",num);
	printf("Ora si chiude il thread %d...\n",num);
	pthread_exit(0);
}


int main(int argc,char * argv[]){
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	
	printf("Parte il main...\n");
	pthread_create(p1,fp1,1);
	pthread_create(p2,fp1,2);
	pthread_create(p3,fp1,3);
	
	printf("Finisco di creare ed ora attendo....");
	
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(p3,NULL);
	
	return 0;
}

