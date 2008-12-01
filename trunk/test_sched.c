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

    void fp1(void * arg){
        int num;
        num=(int)arg;
        printf("Ora e' in funzione il thread %d...\n",num);
        pthread_yield();
        printf("Ora si chiude il thread %d...\n",num);
        pthread_exit(10);
    }


    int main(int argc,char * argv[]){
        pthread_t p1;
        pthread_t p2;
        pthread_t p3;
        pthread_t p4;
    //    pthread_t p5;
        printf("Parte il main...\n");
        int x;//malloc(sizeof(int));
        pthread_create(&p1,fp1,1);
        pthread_create(&p2,fp1,2);
        pthread_create(&p3,fp1,3);
            pthread_create(&p4,fp1,4);	
       //     pthread_create(&p5,fp1,5);
        printf("Finisco di creare ed ora attendo....\n");
        x=-3;
        printf("inizio:%d\n",x);
    //printf("Detach%d\n",pthread_detach(p1));
      printf("1 %d\n",pthread_join(p1,&x));
        printf("Risultato:%d\n",x);
        x=-3;
        printf("2 %d\n", pthread_join(p2,&x));
    printf("Risultato:%d\n",x);
    printf("Detach%d\n",pthread_detach(p2));
    x=-5;
    pthread_join(p3,&x);
    printf("Risultato %d\n",x);
//    printf("Prova%d\n",x);

x=-10;

    pthread_join(p4,&x);
    printf("Risultato %d\n",x);
    pthread_exit(0);
}

