#include <stdio.h>

int AMOUNT = 1;

char *name_funcs[] = {"plus"};

char *description[] = {"Сложение"};

int plus(){

    printf("Введите два число, которые хотите сложить\n");

    int a = 0;
    int b = 0;
    
    int sum = 0;

    printf("---> ");
    scanf("%d", &a);
    printf("\n");

    printf("---> ");
    scanf("%d", &b);
    printf("\n");

    sum = a + b;
    
    printf("Результат ---> %d\n", sum);

    return 0;
}