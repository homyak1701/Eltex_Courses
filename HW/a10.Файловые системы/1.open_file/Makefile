
.PHONY := clean

bin := temp
CC:= gcc

run: temp
		./temp

temp: main.c

		$(CC) main.c -o $(bin) -g

clean: 
		rm -rf temp example.txt