#include <stdio.h>

int main(void)
{
	int c = getchar();
	if(EOF == c){
		printf("feof(stdin) = %d\n", feof(stdin));
		printf("ferror(stdin) = %d\n", ferror(stdin));
		return 1;
	}
	printf("You entered '%c'\n", (char) c);

	//--- Сделать исключание, если у нас ошибка ввода или завершение ввода
	/*int c = getchar();
	if(c == EOF){
		printf("EOF or error\n");
		return 1;
	}
	printf("You entered '%c'\n", (char) c);*/

	// --- Вывод двух симфолов ---
	/*int c = 0;
	c = getchar();
	printf("c = %d\n", c);
	printf("(char) c = %c\n",(char) c);
	c = getchar();
	printf("c = %d\n", c);
        printf("(char) c = %c\n",(char) c);*/
	return 0;
}
