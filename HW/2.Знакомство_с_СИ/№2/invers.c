#include <stdio.h>
#include <stdlib.h>
int main()
{

	int i = 0;
	int size = 0;
	scanf("%d", &size);

	int p1[size];

	printf("Ввод -> ");
	for (int i = 0; i < size; i++)
	{
        	p1[i] = i;
        	printf("%d, ", p1[i]);
	}

	printf("\nВывод ->");
	for (i = 1; i <= size; i++)
	{
		p1[i] = size - i;
		printf("%d, ", p1[i]);
	}
  	return 0;
}
