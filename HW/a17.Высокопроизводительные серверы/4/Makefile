
.PHONY := clean

bin1 := client
bin2 := serv
CC:= gcc

all: gcc

gcc:
		$(CC) client_udp.c -o client_udp -g -pthread -O0
		$(CC) client_tcp.c -o client_tcp -g -pthread -O0
		$(CC) server.c -o $(bin2) -g -pthread -O0

clean: 
		rm -rf *.o *.a client_tcp client_udp serv 
		

