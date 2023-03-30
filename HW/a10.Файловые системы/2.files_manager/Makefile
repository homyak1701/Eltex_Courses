
.PHONY := clean

bin := temp
CC:= gcc

run: 
		./temp

temp: main.c
		$(CC) main.c -o $(bin) -lncurses -L. -ldop_func -g

dop_libs:
		gcc dop_func.c -c
		ar rc libdop_func.a dop_func.o

clean: 
		rm -rf *.o *.a temp 
		

