
CC = gcc 
CFLAGS = -Wall -pedantic
objects = pthread.o context.o struct.o sched.o 
exe=lib_thread

$(exe): $(objects) 
	$(CC) $(CFLAGS) $(objects) -o $(exe)
	  
pthread.o: pthread.c pthread_exit.c 

context.o: pth_context.c pth_context.h config.h pth_errno.h

struct.o: pthread_lib.h pthread.h  pth_struct.h pth_struct.c

sched.o: pthread_sched.h pthread_sched.c

clean: 
	rm $(exe) $(objects) *~ core
