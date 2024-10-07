CC = gcc
CFLAGS = -Wall -g

simple_shell: simple_shell.o builtins/cd.o builtins/exit.o builtins/help.o builtins/history.o
	$(CC) -o simple_shell simple_shell.o builtins/cd.o builtins/exit.o builtins/help.o builtins/history.o

simple_shell.o: simple_shell.c builtins/builtin.h
	$(CC) $(CFLAGS) -c simple_shell.c

builtins/cd.o: builtins/cd.c builtins/builtin.h
	$(CC) $(CFLAGS) -c builtins/cd.c -o builtins/cd.o

builtins/exit.o: builtins/exit.c builtins/builtin.h
	$(CC) $(CFLAGS) -c builtins/exit.c -o builtins/exit.o

builtins/help.o: builtins/help.c builtins/builtin.h
	$(CC) $(CFLAGS) -c builtins/help.c -o builtins/help.o

builtins/history.o: builtins/history.c builtins/builtin.h
	$(CC) $(CFLAGS) -c builtins/history.c -o builtins/history.o

clean:
	rm -f*.o builtins/*.o simple_shell
