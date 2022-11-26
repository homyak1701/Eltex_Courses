#include <stdio.h>

int main()

{
	int io = 0;
	int sflag = 0;
	while((io = getchar()) != EOF)
	{
		if(' ' != io)
		{
			putchar(io);
			sflag = 0;
		} else if (0 == sflag){
			putchar(io);
			sflag = 1;
		}
	}
	return 0;
}
