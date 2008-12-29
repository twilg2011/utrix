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
pthread_mutex_t mux;
pthread_mutex_t mux2=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2;
    void fp1(void * arg){
        int num;
        num=(int)arg;
        printf("Ora e' in funzione il thread %d...\n",num);
   	 printf("Mutex %d, %d\n",num, pthread_mutex_lock(&mux));
        pthread_yield();
   if(num!=4){
   printf("Faccio wait sn thread %d, res:%d\n",num,pthread_cond_wait(&cond,&mux));
   printf("Faccio signal sn thread %d,res %d\n",num,pthread_cond_signal(&cond2));
           }
           else{
       printf("Faccio signal sn thread %d res %d\n",num,pthread_cond_broadcast(&cond));
   printf("Faccio wait sn thread %d res %d\n",num,pthread_cond_wait(&cond2,&mux));
       } 
  printf("Mutex %d sblocco%d\n",num,pthread_mutex_unlock(&mux));
    printf("Ora si chiude il thread %d...\n",num);
        pthread_exit(10);
    }


    int main(int argc,char * argv[]){
        pthread_t p1;
        pthread_t p2;
        pthread_t p3;
        pthread_t p4;
           pthread_cond_t cond1 ;
   printf("inizializzo mutex%d %d\n",EBUSY,pthread_mutex_init(&mux,NULL));
   printf("DESTROY %d\n",pthread_mutex_destroy(&mux));    
    printf("Inizializzo mutex %d\n",pthread_mutex_init(&mux,NULL));
    //    pthread_t p5;
    printf("SECONDO %d\n",pthread_mutex_init(&mux2,NULL));
   printf("SECONDO TERMINA %d\n",pthread_mutex_destroy(&mux2));
  printf("inizializzo COND1 %d\n",pthread_cond_init(&cond,NULL));
    printf("Destroy COND%d\n",pthread_cond_destroy(&cond));
    printf("Inizializzo COND1 %d\n",pthread_cond_init(&cond,NULL));
	pthread_mutex_t mux3;
   printf("TERZO COND%d\n",pthread_cond_init(&cond1,1));
    printf("TERZO COND %d\n",pthread_cond_init(&cond1,NULL));
    printf("SECONDO COND %d\n",pthread_cond_init(&cond2,NULL));
    printf("DESTROY COND %d\n",pthread_cond_destroy(&cond1));   
   /* printf("Destroy3%d,%d\n",EINVAL,pthread_mutex_destroy(&mux3));
   printf("TERZO %d\n",pthread_mutex_init(&mux3,NULL));
   printf("SECONDO ANCORA %d\n",pthread_mutex_init(&mux2,NULL));
    printf("DESTORY ULTIMO %d\n",pthread_mutex_destroy(&mux3));
    */printf("Parte il main...\n");
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

