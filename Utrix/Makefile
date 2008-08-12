
CC = gcc 
CFLAGS = -Wall -pedantic
objects = pthread.o context.o struct.o sched.o 


exe: $(objects) 
     $(CC) $(CFLAGS) $(objects) -o exe
	  
pthread.o: pthread.c pth_struct.h pth_struct.c pthread_exit.c pthread_lib.h pth_struct.h pth_struct.c

context.o: pth_context.c pth_context.h config.h pth_errno.h

struct.o: pthread_lib.h pthread.h  pth_struct.h pth_struct.c

sched.o: pthread_sched.h pthread_sched.c

clean: 
      rm $(exe) $(objects) *~ core
