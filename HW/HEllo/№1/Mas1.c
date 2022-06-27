#include <stdio.h>

int main(void)
{
	int age = 0;
	int schet = 1;

	printf("Input your size:");
	scanf("%d", &age);
	int matrx[age][age];

	for(int i=0; i<age; i++)
	{
		printf(" \n");
		for(int y=0; y<age; y++)
		{
			matrx[i][y] = schet;
			if (9 < schet)
			{
				printf("%d ", matrx[i][y]);
			} else {
				printf("%d  ", matrx[i][y]);
			}
			schet++;
		}
	}
	return 0;
}
