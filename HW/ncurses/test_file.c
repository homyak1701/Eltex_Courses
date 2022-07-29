#include <stdio.h>
#include <string.h>


int main()
{
	int x;
	int y = 2;
	char t[10];

	memset(t, ' ', 10);
	t[5] = 't';
	t[9] = 'y';


	for(int i; i<10; i++)
	{
		printf("%c", t[i]);	
	}
	printf("\n");
	return 0;
}

