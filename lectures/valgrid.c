#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	char *p;
//	int num;


	mallopt(M_CHECK_ACTION, 3);

	p = malloc(1000);
//	p = malloc(2000); //утечка памяти
	p[1000] = 'A';

//	printf("num=%d\n", num);

	if(p == NULL) {
		fprintf(stderr, "malloc() failed");
		exit(EXIT_FAILURE);
	}

	free(p);
	printf("main(): returned from first free() call\n");
//	free(p);
//	printf("main(): returned from second free() call\n");
//	free(p);
//	free(p);
	exit(EXIT_SUCCESS);
}
