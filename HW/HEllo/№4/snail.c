#include <stdio.h> //Для printf
#include <math.h>  //Для ceil, ceilf, ceill


int main(void)
{
	int age = 0;
	int schet = 1;

	printf("Enter size matrx");
	scanf("%d", &age);

	printf("%d \n",age%2);
	int mas[age][age];
	mas[age/2][age/2] = age*age;
	for(int i = 0; i < 3; i++ )
	{
			for(int i1 = i; i1 < age-i; i1++) //вверхняя горизонталь
			{
				mas[i][i1] = schet;
				schet++;
			}
			for(int i2 = i+1; i2 < age-i; i2++) //правая вертикаль
			{
				mas[i2][age-i-1] = schet;
				schet++;
			}
			for(int i3 = age-i-2; i3 > i; i3--) //нижняя горизонталь
			{
				mas[age-i][i3] = schet;
				schet++;
			{
			for(int i4 = age-i; i4 > i+1; i4--) //левая вертикаль
			{
					mas[i4][i] = schet;
					schet++;
			}
	}

	for(int z1 = 0; z1 < age; z1++)
	{
		printf("\n");
		for(int z2 = 0; z1 < age; z2++)
		{
			if (9 < schet)
                        {
                                printf("%d ", mas[z1][z2]);
                        } else {
                                printf("%d  ", mas[z1][z2]);
                        }
                        schet++;

		}
	}
	return 0;
}}
