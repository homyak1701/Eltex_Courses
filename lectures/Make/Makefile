
bin := temp
code := 1.c
headers := my.h

.PHONY := clean;

all: $(bin) clean

$(bin): $(code) $(headers)
		gcc $(code) -o $(bin)

clean:
		rm -rf $(bin)