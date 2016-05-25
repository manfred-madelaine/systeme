CC = gcc

CFLAGS = -Wall #options du compilateur

OBJS = shell_final.o my_shell.o my_history.o my_cat.o my_copy.o my_pipe.o my_find.o

PROG = My_Shell 


all : $(OBJS) 
	$(CC) -o $(PROG) $(OBJS)

shell_final.o : ./shell_final.c
	$(CC) $(CFLAGS)  -c ./shell_final.c

my_shell.o : ./my_shell.c
	$(CC) $(CFLAGS) -c $< 

my_history.o : ./my_history.c
	$(CC) $(CFLAGS) -c $<

my_cat.o : ./my_cat.c
	$(CC) $(CFLAGS) -c $<

my_copy.o : ./my_copy.c
	$(CC) $(CFLAGS) -c $<

my_pipe.o : ./my_pipe.c
	$(CC) $(CFLAGS) -c $<

my_find.o : ./my_find.c
	$(CC) $(CFLAGS) -c $<

clean :
	-rm *.o *~ $(PROG)

