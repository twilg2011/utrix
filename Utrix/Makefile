
CC = gcc
CFLAGS = -Wall -pedantic -c
objects = pthread.o context.o struct.o sched.o syncr.o
exe=lib_thread

$(exe): $(objects)
	$(CC) $(CFLAGS) $(objects) -o $(exe)
          
pthread.o: pthread.c pthread_exit.c
	$(CC) $(CFLAGS) pthread.c

context.o: pth_context.c pth_context.h config.h pth_errno.h
	$(CC) $(CFLAGS) pth_context.c

struct.o: pthread_lib.h pthread.h  pth_struct.h pth_struct.c
	$(CC) $(CFLAGS) pth_struct.c

sched.o: pthread_sched.h pthread_sched.c
	$(CC) $(CFLAGS) pthread_sched.c

syncr.o: pth_syncr.c pth_syncr.h syncr.s
	$(CC) $(CFLAGS) pth_syncr.c

clean: 
	rm $(exe) $(objects) *~ core

