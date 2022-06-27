#include <stdio.h>


struct test
{
	char a;
	int b;
};

struct test2
{
	char a2;
	int b2;
}__attribute__((packed)); //убрать выравнивание

int main(void)
{
	struct test my_test;
	struct test2 my_test2;

	printf("\nЗначение a до присваивания = %c(&a= %p)\nЗначение b до присваивания =%d (&b = %p)\n",(char)my_test.a, &my_test.a, my_test.b, &my_test.b);
	printf("\nЗначение a2 до присваивания = %c (&a2= %p) \nЗначение b2 до присваивания =%d (&b2 = %p)\n",(char)my_test2.a2, &my_test2.a2, my_test2.b2, &my_test2.b2);

	my_test.a = 'A';
	my_test.b = 123456;

	my_test2.a2 = 'A';
        my_test2.b2 = 123456;

	printf("\nЗначение a после присваивания = %c(&a= %p)\nЗначение b до присваивания =%d (&b = %p)\n",(char)my_test.a, &my_test.a, my_test.b, &my_test.b);
        printf("\nЗначение a2 после присваивания = %c(&a2= %p)\nЗначение b2 до присваивания =%d (&b2 = %p)\n",(char)my_test2.a2, &my_test2.a2, my_test2.b2, &my_test2.b2);

}
