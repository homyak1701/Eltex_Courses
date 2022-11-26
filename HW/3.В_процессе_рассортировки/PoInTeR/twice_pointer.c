#include <stdio.h>


int main(void)
{
	char a = 'A';
	char b = 'B';
	char *ptr1;
	char **ptr2;

	ptr1 = &a;
	ptr2 = &ptr1;

	printf("До изменений\n");

	printf("ptr1 = %p\n", ptr1); // Cсылка на а
	printf("*ptr1 = %c\n", *ptr1); // значение а
	printf("&ptr1 = %p\n", &ptr1); // ссылка на указатель а
	printf("ptr2 = %p\n", ptr2); // указатель на ссылку указателя а
	printf("*ptr2 = %p\n", *ptr2); // значение указателя на сслку a
	printf("**ptr2 = %c\n", **ptr2); // значение указателя на сслку a

	printf("После изменений\n");

	*ptr2 = &b;

	printf("ptr1 = %p\n", ptr1); // Cсылка на а
        printf("*ptr1 = %c\n", *ptr1); // значение а
        printf("&ptr1 = %p\n", &ptr1); // ссылка на указатель а
        printf("ptr2 = %p\n", ptr2); // указатель на ссылку указателя а
        printf("*ptr2 = %p\n", *ptr2); // значение указателя на сслку a
        printf("**ptr2 = %c\n", **ptr2); // значение указателя на сслку a

}
