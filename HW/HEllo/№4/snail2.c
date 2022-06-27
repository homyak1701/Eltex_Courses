#include <stdio.h> //Для printf
#include <math.h>  //Для ceil, ceilf, ceill


int main(void)
{
	int age = 0;
	int schet = 1;
	int N;
	int max;

	printf("Enter size matrx --- ");
	scanf("%d", &age);
	int mas[age][age];

	mas[age/2][age/2] = age*age;

	max = age/2;
	printf("max = %d \n", max);

	for(int i = 0; i < max; i++)
	{
		for(int i1 = i; i1 < age-i; i1++) //вверхняя горизонталь
		{
			mas[i][i1] = schet;
			schet++;
		}
		for(int i2 = i+1; i2 < age-i; i2++) //правая вертикаль
		{
			mas[i2][age-1-i] = schet;
			schet++;
		}
		for(int i3 = age-2-i; i3 >= i; i3--) //нижняя горизонталь
		{
			mas[age-i-1][i3] = schet;
			schet++;
		}
		for(int i4 = age-i-2; i4 >= i+1; i4--) //левая вертикаль
		{
			mas[i4][i] = schet;
			schet++;
		}
	}

	for(int i=0; i<age; i++)
	{
		for(int y = 0; y<age; y++)
		{
			if (9 > mas[i][y])
			{
				printf("%d  ", mas[i][y]);
			} else {
				printf("%d ", mas[i][y]);
			}
		}
		printf("\n");
	}
	return 0;
}
