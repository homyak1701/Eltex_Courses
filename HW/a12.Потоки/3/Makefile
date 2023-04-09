
.PHONY := clean

bin := temp
CC:= gcc
file_lib:= dop_func
main_file:= main

temp: $(main_file).c
		$(CC) $(main_file).c -o $(bin) -L. -ldop_func -g

dop_libs: 
		gcc -fPIC $(file_lib).c -c
		gcc -shared $(file_lib).o -o libdop_func.so
		rm -rf *.o 

clean: 
		rm -rf *.o *.a *.so temp
		

