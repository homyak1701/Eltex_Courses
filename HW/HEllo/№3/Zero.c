#include <stdio.h>

int main()
{
	int size = 0;
	printf("Enter you size mas --- ");
	scanf("%d", &size);
	int k=2;

	int mas[size][size];
	for (int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			mas[x][y] = 0;
		}
		for(int z = 1; z < k; z++)
                {
                	mas[x][size-z] = 1;
                }
		k++;
	}

	for (int x = 0;x < size; x++)
	{
		for(int y=0; y < size; y++)
		{
			printf("%d ", mas[x][y]);
		}
		printf("\n");
	}
	return 0;
}
