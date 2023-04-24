
.PHONY := clean

bin1 := client
bin2 := serv
CC:= gcc

all: gcc

gcc:
		$(CC) client.c -o $(bin1) -g -lncurses
		$(CC) server.c -o $(bin2) -g -lncurses

clean: 
		rm -rf *.o *.a client serv 
		

