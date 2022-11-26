#include <stdio.h>
#include "calculator.h"


int main(void)
{
	int a, c, result;

	a = 10;
	c = 5;
	
	result = plus(a,c);
	printf("plus = %d\n", result);

	result = minus(a,c);
	printf("minus = %d\n", result);

	result = multiply(a,c);
	printf("multiply = %d\n", result);

	result = devide(a,c);
	printf("devide = %d\n", result);
}
