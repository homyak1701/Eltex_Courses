
.PHONY := clean

bin := temp

CC:= gcc

temp: plus.c main.c
		$(CC) plus.c minus.c multiply.c division.c -c
		ar rc libcalculator.a plus.o minus.o multiply.o division.o
		$(CC) main.c -o temp -L. -lcalculator -g

clean: 
		rm -rf *.o *.a temp
		

