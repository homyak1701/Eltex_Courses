
.PHONY := clean

bin := temp
CC:= gcc

#set environment LD_LIBRARY_PATH ./libs/:$LD_LIBRARY_PATH
#run libs/libdevision.so libs/libminus.so libs/libmult.so libs/libplus.so

run: 
		LD_LIBRARY_PATH=./libs/:$$LD_LIBRARY_PATH ./temp libs/libdevision.so libs/libminus.so libs/libmult.so libs/libplus.so libs/libtwice.so 

temp: main.c

		$(CC) main.c -o $(bin) -Llibs/. -ldevision -lminus -lmult -lplus -g

dop_libs:
		 
		cd libs/ && $(CC) -fPIC devision.c -c
		cd libs/ && $(CC) -fPIC minus.c -c
		cd libs/ && $(CC) -fPIC mult.c -c
		cd libs/ && $(CC) -fPIC plus.c -c
		cd libs/ && $(CC) -fPIC twice.c -c

		cd libs/ && gcc -shared devision.o -o libdevision.so
		cd libs/ && gcc -shared minus.o -o libminus.so
		cd libs/ && gcc -shared mult.o -o libmult.so
		cd libs/ && gcc -shared plus.o -o libplus.so
		cd libs/ && gcc -shared twice.o -o libtwice.so

		cd libs/ && rm -rf *.o 

clean: 
		rm -rf *.o *.a temp 
		cd libs/ && rm -rf *.o *.so temp
		cd ..
		

