#include <stdio.h>


int main(void)
{
	int SIGNS = 255;
	int i = 0;
	int sch1 = 1;
	int sch2 = 0;
	int mas[SIGNS*2];
	int io = 0;

	for(i = 0; i < SIGNS*2; ++i)
	{
		mas[i] = 0;
	}

	while(EOF != (io = getchar()))
	{
		for(i = 0; i < SIGNS*2-1; i=i+2)
		{
			if((mas[i] == io) && (0 != mas[i+1]))
			{
				++mas[i+1];
				sch1 = 0;
			} else if((0 != sch1) && (SIGNS*2-2 == i) && (io != '\n'))
			{
				mas[sch2] = io;
				mas[sch2+1] = 1;
				sch2 = sch2+2;
			}
		}
		sch1 = 1;
	}
	for(i = 0; i < sch2; i=i+2)
	{
		printf("\nСимвол ---%c--- повторяется ---%d--- раз\n", (char)mas[i], mas[i+1]);
	}
	return 0;
}
