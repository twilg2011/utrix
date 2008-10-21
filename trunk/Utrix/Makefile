
CC = gcc
CFLAGS = -Wall -pedantic -c
objects = pthread.o pth_context.o pth_struct.o pthread_sched.o pth_syncr.o main.c
exe=lib_thread

$(exe): $(objects)
	$(CC) $(objects) -o $(exe)
          
pthread.o: pthread.c
	$(CC) $(CFLAGS) pthread.c

pth_context.o: pth_context.c pth_context.h config.h pth_errno.h
	$(CC) $(CFLAGS) pth_context.c

pth_struct.o: pthread_lib.h pthread.h  pth_struct.h pth_struct.c
	$(CC) $(CFLAGS) pth_struct.c

pthread_sched.o: pthread_sched.h pthread_sched.c
	$(CC) $(CFLAGS) pthread_sched.c

pth_syncr.o: pth_syncr.c pth_syncr.h syncr.s
	$(CC) $(CFLAGS) pth_syncr.c

clean: 
	rm $(exe) $(objects) *~ core

