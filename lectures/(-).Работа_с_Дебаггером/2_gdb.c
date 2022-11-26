#include <stdio.h>

long test(long a,long b,long c,long d,long e,long f, long g,long h,long i)
{

	long sum = 0;

	sum = a + b + c + d + e + f + g + h + i;

	return sum;
}

int main(void)
{
	long a,b,c,d,e,f,g,h,i,j;

	a = 5;
	b = 6;
	c = 7;
	d = 8;
	e = 9;
	f = 10;
	i = 11;

	j = test(a,b,c,d,e,f,g,h,i);
	printf("sum = %ld\n", j);

	return 0;
}
