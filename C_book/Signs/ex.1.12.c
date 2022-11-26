#include <stdio.h>

int main(void)
{
	int io;
	while((io = getchar()) != EOF)
	{
		if((' ' == io) || ('\t' == io))
		{
			printf("\n");
		} else{
		putchar(io);
		}
	}
	return 0;
}
