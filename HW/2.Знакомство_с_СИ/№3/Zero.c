#include <stdio.h>

int main()
{
	int size = 0;
	printf("Enter you size mas --- ");
	scanf("%d", &size);
	int x = 0;
	int y = 0;
	int z = 0;
	//- ограничетель k, который будет помогать конец массива строки
	//и каждый такт будет сдвигаться налево
	int k=1;
	int mas[size][size];

	for (x = 0; x < size; x++){
		//заполняем слева-напрво нулями до ограничителя
		for(y = 0; y < size-k; y++){
			mas[x][y] = 0;
		}
		//заполняем справа-налево единицами до ограничителя
		for(z = 1; z < k+1; z++){
                	mas[x][size-z] = 1;
                }
		k++;
	}

	// вывод заполненной матрицы
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
