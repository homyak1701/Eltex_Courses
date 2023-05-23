
.PHONY := clean

bin1 := client
bin2 := serv
CC:= gcc

all: gcc

gcc:
		$(CC) client.c -o $(bin1) -g 
		$(CC) server.c -o $(bin2) -g

clean: 
		rm -rf *.o *.a client serv 
		

