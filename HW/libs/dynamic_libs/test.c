// компиляиця --> gcc test.c -o Temp -ldl

#include <stdio.h>
#include <dlfcn.h>


int main(void)
{

	int a, c, result;

	a = 10;
	c = 5;

	void *handel; //указатель на библиотеку
	int (*plus_f)(int,int); //указатель на функцию plus;
	int (*minus_f)(int,int); //указатель на функцию plus;
	int (*devide_f)(int,int); //указатель на функцию plus;
	int (*multiply_f)(int,int); //указатель на функцию plus;



	char* name;
        scanf("%s", name);
	cahr *name2 = "./" + name;
        handel = dlopen(name, RTLD_LAZY);

	plus_f = dlsym(handel, "plus");
	minus_f = dlsym(handel, "minus");
	devide_f = dlsym(handel, "devide");
	multiply_f = dlsym(handel, "multiply");


	printf("plus = %d\n", (*plus_f)(a,c));
	printf("minus = %d\n", (*minus_f)(a,c));
	printf("multiply = %d\n", (*devide_f)(a,c));
	printf("devide = %d\n", (*multiply_f)(a,c));

	return 0;
}
