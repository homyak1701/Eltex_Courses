#include <stdio.h>

int main(void)
{
	int io = 0;
	int sch = 1;
	while (EOF != (io = getchar()))
	{
		if((' ' == io) || ('\n' == io) || ('\t' == io))
		{
			sch = 0;
			printf("\n");
			sch++;
		} else if (0 < sch){
                	printf("_");
		}
	}

}
