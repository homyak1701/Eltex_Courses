
.PHONY := clean

bin := temp
CC:= gcc

temp: main.c libdop.a
		$(CC) main.c -o $(bin) -L. -ldop

dop_libs: name_func.c
		$(CC) name_func.c -c
		ar rc libdop.a name_func.o
		rm -rf name_func.o

clean: 
		rm -rf *.o *.a temp
		

