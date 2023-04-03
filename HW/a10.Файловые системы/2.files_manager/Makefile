
.PHONY := clean

bin := temp
CC:= gcc

run: 
		./temp

temp: dop_func.c main.c
		gcc dop_func.c -c -g
		ar rc libdop_func.a dop_func.o
		$(CC) main.c -o $(bin) -lncurses -L. -ldop_func -g -O0

clean: 
		rm -rf *.o *.a temp 
		

