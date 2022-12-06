// Вывести квадратичную матрицу по заданному N

#include <stdio.h>

int main(void)
{
	int age;

	printf("Input your size: ");
	scanf("%d", &age);

	for(int i=0 ; i<age*age ; i++){
		// проверка на границу квадрата
		if((i % age) == 0){
			printf("\n");
		}
		//- условия для правсильного отображения матрицы
		//- если ее убрать, то после ввода 4 она перестанет
		//быть квадратной
		if(( (i / 10) == 0 ) && (i != 9)){
			printf("%d  ", i+1);
		} else {
			printf("%d ", i+1);
		}
	}

	printf("\n\n");

	return 0;
}
