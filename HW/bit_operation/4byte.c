#include <stdio.h>


int main(void)
{
	int i = 0;
	int a = 0xDDCCBBAA;
	int g = 0;

	printf("a равно %x\n", a);

	for(i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 3:
				printf("%d бит --- %x\n",i, ((a >> 8*i) & 0x000000FF));
				a = a ^ 0x00005500;
				break;
			default:
				printf("%d бит --- %x\n",i, ((a >> 8*i) & 0x000000FF));
				break;
		}
	}
	printf("a равно %x\n", a);
	return 0;
}
