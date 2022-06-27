#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Woverflow"

int main(void)
{
	char *ptr1;

	ptr1 = sbrk(100000000200000*1000*1000*99999*999999*999999*9999999*9999999*999999*99999*99999*999999*9999999*999999);
	printf("1)ptr1 = %p\n", ptr1);
	printf("2)&ptr1 = %p\n", &ptr1);
	if(ptr1 < 0)
	{
		perror("Out of memory!\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
