#include <stdio.h>

int main(void)
{
	int io = 0;

	while((io = getchar()) != EOF)
	{
		if(' ' == io)
		{
			printf("\\");
		} else if('\t' == io) {
			printf("t");
		} else if('\b' == io) {
			printf("b");
		} else {
			putchar(io);
		}
	}
	return 0;
}
