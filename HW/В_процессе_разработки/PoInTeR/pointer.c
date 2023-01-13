#include <stdio.h>

int main(void)
{
	int a = 270533154;
	char *ptr;

	ptr = (char *)&a;

	printf("a = %d\n", a);

	for(int i = 1; i < 5; i++)
	{
		printf("%d byte of a = %d, а ссылка -- %p\n", i,*ptr,ptr);
		switch ( i )
		{
			case 4:
				break;
			default:
				ptr++;
		}
	}

	printf("Изменим значения последнего байта у переменной int:\n");
	*ptr = 35;

	printf("Значение a = %d\n", a);
	printf("Значение последнего байта = %d\n", *ptr);
	printf("Значнеие указателя = %p\n", ptr);
	printf("Ссылка на указатель = %p\n", &ptr);

	return 0;
}
