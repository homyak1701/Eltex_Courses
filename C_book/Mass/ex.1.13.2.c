#include <stdio.h>

#define MAXLEN 10

int main(void)
{
	int i = 0;
	int c = 0;
	int ncw = 0;
	int wlength[MAXLEN];

	for (i = 0;i < MAXLEN; ++i)
	{
		wlength[i]=0;
	}
// Считаем сколько слов у нас определенной длины
	while((c = getchar()) != EOF)
	{
		if (c == ' ' || c == '\n' || c == '\t')
		{
			if(ncw >= MAXLEN)
			{
				wlength[0]+=1;
			} else {
				wlength[ncw]+=1;
				ncw = 0;
			}
		} else ++ncw;
	}
	printf("\n");
	for (i = 0; i < MAXLEN; ++i)
	{
		printf("%d : ",i);
		for(int y=0; y < wlength[i]; ++y)
		{
			printf("|");
		}
		printf(" > %d\n", wlength[i]);
	}
	return 0;
}
